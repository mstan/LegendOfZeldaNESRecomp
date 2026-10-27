"""Muted portal presentation checks using real entrances, exits and pixels.

Pillow is used only to inspect screenshots, never to change the game's output.
"""
import argparse
import json
import os
from pathlib import Path
import socket
import subprocess
import time
from PIL import Image


def main():
    ap = argparse.ArgumentParser()
    for n in ('exe', 'rom', 'state', 'out'):
        ap.add_argument('--'+n, type=Path, required=True)
    ap.add_argument('--port', type=int, default=27587)
    a = ap.parse_args()
    a.out.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, SDL_AUDIODRIVER='dummy', SDL_VIDEODRIVER='dummy',
               SDL_RENDER_DRIVER='software', NESRECOMP_NO_LAUNCHER='1')
    for key in ('APPDATA', 'LOCALAPPDATA', 'TEMP', 'TMP'):
        d = a.out/key.lower()
        d.mkdir(exist_ok=True)
        env[key] = str(d.resolve())
    config = a.out/'config.ini'
    config.write_text('[Display]\nFullscreen=0\nVSync=0\n[Audio]\nVolume=0\n'
                      '[Input]\nPlayer1Source=0\nPlayer2Source=0\n', encoding='utf-8')
    kwargs = {}
    if os.name == 'nt':
        si = subprocess.STARTUPINFO()
        si.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        si.wShowWindow = 0
        kwargs = dict(startupinfo=si, creationflags=subprocess.CREATE_NO_WINDOW)
    results, transcript = {}, []
    with (a.out/'game.log').open('w', encoding='utf-8') as log:
        p = subprocess.Popen([str(a.exe.resolve()), str(a.rom.resolve()), '--hidden',
                              '--no-save', '--tcp', str(a.port), '--config', str(config.resolve()),
                              '--world-view', 'seamless'], cwd=a.exe.parent, env=env,
                             stdout=log, stderr=subprocess.STDOUT, **kwargs)
        stream = sock = None
        try:
            deadline = time.monotonic()+15
            while time.monotonic() < deadline:
                if p.poll() is not None:
                    raise RuntimeError(f'Game exited {p.returncode}')
                try:
                    sock = socket.create_connection(('127.0.0.1', a.port), timeout=1)
                    sock.settimeout(20)
                    stream = sock.makefile('rwb')
                    break
                except OSError:
                    time.sleep(.1)
            assert stream is not None, 'TCP startup timed out'

            def cmd(command, **fields):
                request = dict(cmd=command, id=1, **fields)
                stream.write((json.dumps(request)+'\n').encode())
                stream.flush()
                reply = json.loads(stream.readline())
                transcript.append(dict(request=request, response=reply))
                assert reply.get('ok'), reply
                return reply

            def frames(n):
                start = cmd('state')['frame']
                deadline = time.monotonic()+max(10, n/20)
                while time.monotonic() < deadline:
                    if cmd('state')['frame'] >= start+n:
                        return
                    time.sleep(.015)
                raise RuntimeError('No frame progress')

            def shot(name):
                path = (a.out/(name+'.png')).resolve()
                receipt = cmd('screenshot', layer='picture', path=str(path))
                with Image.open(path) as im:
                    im = im.convert('RGB')
                    w, h = im.size
                    # The old compositor's entire scene was inside this box.
                    # Count actual nonblack content outside it, below the HUD.
                    box = ((w-256)//2, (h-240)//2, (w+256)//2, (h+240)//2)
                    pixels = im.load()
                    outside = sum(max(pixels[x, y]) > 40
                                  for y in range(64, h) for x in range(w)
                                  if not (box[0] <= x < box[2] and box[1] <= y < box[3]))
                return dict(receipt=receipt, outside_native_scene_pixels=outside,
                            world=cmd('zelda_world'), state=cmd('zelda_state'))

            def monitor(name, done):
                samples, last, restored = [], None, False
                deadline = time.monotonic()+25
                while time.monotonic() < deadline:
                    w = cmd('zelda_world')
                    key = (w['game_mode'], w['updating'], w['submode'], w['presentation'])
                    assert w['size'] == [768, 480], w
                    assert w['presentation'] in ('world', 'world-walk', 'subroom',
                                                 'subroom-walk', 'portal-retained'), w
                    if key != last:
                        samples.append(shot(f'{name}-{len(samples):02d}'))
                        last = key
                    if w['presentation'] == 'portal-retained' and not restored:
                        cmd('menu', open=True)
                        stable = cmd('zelda_world')
                        if stable['presentation'] == 'portal-retained':
                            before = shot(name+'-before-restore')
                            path = (a.out/(name+'-loading.state')).resolve()
                            cmd('save_state', path=str(path))
                            cmd('load_state', path=str(path))
                            after = shot(name+'-after-restore')
                            with Image.open(before['receipt']['path']) as left, Image.open(after['receipt']['path']) as right:
                                assert left.tobytes() == right.tobytes(), (name, 'Portal source changed after load')
                            assert after['world']['camera'] == before['world']['camera']
                            results[name+'-state'] = dict(before=before, after=after)
                            restored = True
                        cmd('menu', open=False)
                    if done(w):
                        break
                    time.sleep(.02)
                else:
                    raise AssertionError((name, samples[-1] if samples else None))
                results[name] = samples
                return samples

            cmd('menu', open=True)
            cmd('load_state', path=str(a.state.resolve()))
            cmd('window_size', w=2304, h=1440)
            shot('start')
            entrance = cmd('zelda_world_entrance', room=0x77)
            cmd('zelda_world_teleport', room=0x77, x=entrance['x'], y=entrance['y'], gear=True)
            cmd('menu', open=False)
            cmd('action', name='up', frames=45)
            cave = monitor('cave-entry', lambda w: w['game_mode'] == 11 and w['updating'])
            assert any(s['world']['presentation'] == 'world-walk' and
                       s['outside_native_scene_pixels'] > 500 for s in cave), cave
            frames(20)
            cmd('menu', open=True)
            results['cave'] = shot('cave-interior')
            assert results['cave']['outside_native_scene_pixels'] > 10000
            results['cave_resizes'] = []
            for width, height in ((3840, 720), (600, 1200), (2304, 1440)):
                cmd('window_size', w=width, h=height)
                s = shot(f'cave-{width}x{height}')
                assert s['receipt']['size'] == [width//3, height//3], s
                assert s['outside_native_scene_pixels'] > 1000, s
                results['cave_resizes'].append(s)
            # Same native room, inventory and portal can be restored normally.
            cave_state = (a.out/'cave.state').resolve()
            cmd('save_state', path=str(cave_state))
            cmd('load_state', path=str(cave_state))
            restored = shot('cave-restored')
            assert restored['receipt']['size'] == [768, 480]
            assert restored['world']['presentation'] == 'subroom'
            results['cave_restored'] = restored
            cmd('menu', open=False)
            # Native dialog temporarily halts Link. Do not spend the entire
            # exit input while the old man's message is still being written.
            deadline = time.monotonic()+12
            while cmd('zelda_state')['link_state'] & 0x40:
                assert time.monotonic() < deadline, 'Cave dialog did not finish'
                time.sleep(.05)
            cmd('action', name='down', frames=70)
            monitor('cave-exit', lambda w: w['game_mode'] == 5)
            assert cmd('zelda_state')['room'] == 0x77
            cmd('menu', open=True)
            entrance = cmd('zelda_world_entrance', room=0x37)
            cmd('zelda_world_teleport', room=0x37, x=entrance['x'], y=entrance['y'], gear=True)
            cmd('menu', open=False)
            cmd('action', name='up', frames=45)
            monitor('dungeon-entry', lambda w: w['game_mode'] == 5 and w['scene_level'] == 1)
            frames(20)
            cmd('menu', open=True)
            results['dungeon'] = shot('dungeon-interior')
            assert results['dungeon']['world']['scene_level'] == 1
            assert results['dungeon']['outside_native_scene_pixels'] > 500
            cmd('zelda_world_teleport', room=0x73, x=120, y=213)
            cmd('menu', open=False)
            cmd('action', name='down', frames=70)
            monitor('dungeon-exit', lambda w: w['game_mode'] == 5 and w['scene_level'] == 0)
            assert cmd('zelda_state')['room'] == 0x37
            assert cmd('zelda_world')['failures'] == 0
            results['status'] = 'pass'
            cmd('quit')
            p.wait(timeout=15)
        finally:
            if stream:
                stream.close()
            if sock:
                sock.close()
            if p.poll() is None:
                p.terminate()
                p.wait(timeout=10)
            (a.out/'results.json').write_text(json.dumps(results, indent=2)+'\n', encoding='utf-8')
            (a.out/'tcp.json').write_text(json.dumps(transcript, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(dict(status=results.get('status'), phases={k: len(v) for k, v in results.items()
                                                            if isinstance(v, list)})))


if __name__ == '__main__':
    main()
