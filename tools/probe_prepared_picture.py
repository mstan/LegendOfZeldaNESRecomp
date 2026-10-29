"""Hidden, muted real-game checks for prepared presentation boundaries.

Use disposable states from this build and matching mods/view configuration.
This never writes native saves. Run under the shared benchmark reservation.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import socket
import subprocess
import time


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    for name in ('exe', 'rom', 'state', 'out'):
        ap.add_argument('--'+name, type=Path, required=True)
    ap.add_argument('--advanced-state', type=Path)
    ap.add_argument('--view', choices=('world', 'diorama', 'first-person', 'stock', 'hd'), required=True)
    ap.add_argument('--require-reuse', action='store_true')
    ap.add_argument('--require-terrain', action='store_true')
    a = ap.parse_args()
    a.out.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, SDL_AUDIODRIVER='dummy', SDL_VIDEODRIVER='dummy',
               SDL_RENDER_DRIVER='software', NESRECOMP_NO_LAUNCHER='1')
    for key in ('APPDATA', 'LOCALAPPDATA', 'TEMP', 'TMP'):
        p = a.out/key.lower()
        p.mkdir(exist_ok=True)
        env[key] = str(p.resolve())
    config = a.out/'config.ini'
    config.write_text('[Display]\nFullscreen=0\nVSync=0\n[Audio]\nEnabled=1\nVolume=0\n[Input]\nPlayer1Source=0\nPlayer2Source=0\n', encoding='utf-8')
    with socket.socket() as s:
        s.bind(('127.0.0.1', 0))
        port = s.getsockname()[1]
    command = [str(a.exe.resolve()), str(a.rom.resolve()), '--hidden', '--no-save',
               '--tcp', str(port), '--config', str(config.resolve()),
               '--mods-root', str((a.exe.parent/'mods').resolve())]
    if a.view in ('diorama', 'first-person'):
        command += ['--voxel', a.view]
    si = None
    if os.name == 'nt':
        si = subprocess.STARTUPINFO()
        si.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        si.wShowWindow = 0
    transcript, results = [], dict(command=command, view=a.view)
    with (a.out/'game.log').open('w', encoding='utf-8') as log:
        p = subprocess.Popen(command, cwd=a.exe.parent, env=env, stdout=log,
                             stderr=subprocess.STDOUT, startupinfo=si,
                             creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0))
        sock = stream = None
        try:
            deadline = time.monotonic()+30
            while time.monotonic() < deadline:
                if p.poll() is not None:
                    raise RuntimeError('Game exited before TCP startup')
                try:
                    sock = socket.create_connection(('127.0.0.1', port), timeout=1)
                    sock.settimeout(20)
                    stream = sock.makefile('rwb')
                    break
                except OSError:
                    time.sleep(.05)
            assert stream is not None

            def ask(cmd, optional=False, **fields):
                request = dict(cmd=cmd, id=len(transcript)+1, **fields)
                stream.write((json.dumps(request)+'\n').encode())
                stream.flush()
                answer = json.loads(stream.readline())
                transcript.append(dict(request=request, response=answer))
                if not optional:
                    assert answer.get('ok'), answer
                return answer

            def shot(name):
                path = (a.out/(name+'.png')).resolve()
                response = ask('screenshot', layer='picture', path=str(path))
                results.setdefault('pictures', {})[name] = dict(
                    response=response, sha256=hashlib.sha256(path.read_bytes()).hexdigest())
                return results['pictures'][name]['sha256']

            ask('menu', open=True)
            ask('load_state', path=str(a.state.resolve()))
            ask('window_size', w=1280, h=720)
            time.sleep(.15)
            results['initial_state'] = ask('state')
            results['guest'] = ask('zelda_state')
            assert results['guest']['mode'] == 5, results['guest']
            initial = shot('initial')
            before = ask('zelda_presentation', optional=not a.require_reuse)
            for i in range(4):
                assert shot('repeat-'+str(i)) == initial, 'paused picture changed'
            after = ask('zelda_presentation', optional=not a.require_reuse)
            if a.require_reuse:
                assert after['builds'] == before['builds'], (before, after)
                assert after['hits'] > before['hits'], (before, after)
            results['reuse'] = dict(before=before, after=after)
            frame = results['initial_state']['frame']
            for w, h in ((1536, 720), (600, 1200), (1920, 540)):
                ask('window_size', w=w, h=h)
                time.sleep(.15)
                shot('resize-'+str(w)+'x'+str(h))
                assert ask('state')['frame'] == frame, 'paused resize advanced guest'
            ask('window_size', w=1280, h=720)
            time.sleep(.15)
            ask('load_state', path=str(a.state.resolve()))
            assert shot('restored') == initial, 'load did not rebuild original picture'
            if a.view == 'world':
                ask('zelda_world_teleport', room=0x78, x=120, y=141, gear=True)
                assert ask('state')['frame'] == frame
                moved = shot('teleported')
                assert moved != initial, 'same-frame teleport retained old picture'
                state_b = (a.out/'same-frame-b.state').resolve()
                ask('save_state', path=str(state_b))
                ask('load_state', path=str(a.state.resolve()))
                assert shot('same-frame-loaded-a') == initial
                ask('load_state', path=str(state_b))
                assert ask('state')['frame'] == frame
                assert shot('same-frame-loaded-b') == moved
            if a.advanced_state:
                ask('load_state', path=str(a.advanced_state.resolve()))
                advanced = shot('advanced')
                assert ask('state')['frame'] != frame
                ask('load_state', path=str(a.state.resolve()))
                assert shot('advanced-return') == initial
                ask('load_state', path=str(a.advanced_state.resolve()))
                assert shot('advanced-reload') == advanced
            if a.require_terrain:
                before_live=ask('state')
                ask('menu',open=False)
                deadline=time.monotonic()+15
                while ask('state')['frame']<before_live['frame']+20:
                    assert time.monotonic()<deadline,'gameplay did not advance'
                    time.sleep(.03)
                ask('menu',open=True)
                live=ask('zelda_presentation')
                assert live['terrain_builds']>0 and live['terrain_hits']>0,live
                assert 0<live['terrain_bytes']<=36*1024*1024,live
                results['live_terrain_reuse']=dict(before=before_live,after=ask('state'),cache=live)
            results['final_state'] = ask('state')
            ask('quit')
            p.wait(timeout=10)
            assert p.returncode == 0
            results['status'] = 'pass'
        finally:
            if stream:
                stream.close()
            if sock:
                sock.close()
            if p.poll() is None:
                p.terminate()
                p.wait(timeout=10)
            (a.out/'tcp.json').write_text(json.dumps(transcript, indent=2)+'\n', encoding='utf-8')
            (a.out/'results.json').write_text(json.dumps(results, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(results))


if __name__ == '__main__':
    main()
