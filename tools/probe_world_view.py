"""Muted, hidden actual-game probe for the development world view.

The supplied state is disposable. This never imports or writes native saves.
"""
import argparse
import json
import os
from pathlib import Path
import socket
import subprocess
import time


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exe', type=Path, required=True)
    ap.add_argument('--rom', type=Path, required=True)
    ap.add_argument('--state', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--port', type=int, default=27581)
    args = ap.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, SDL_AUDIODRIVER='dummy', SDL_VIDEODRIVER='dummy',
               SDL_RENDER_DRIVER='software', NESRECOMP_NO_LAUNCHER='1')
    for key in ('APPDATA', 'LOCALAPPDATA', 'TEMP', 'TMP'):
        path = args.out / key.lower()
        path.mkdir(exist_ok=True)
        env[key] = str(path.resolve())
    config = args.out / 'config.ini'
    config.write_text('[Display]\nFullscreen=0\nVSync=0\n[Audio]\nEnabled=1\nVolume=0\n', encoding='utf-8')
    kwargs = {}
    if os.name == 'nt':
        si = subprocess.STARTUPINFO()
        si.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        si.wShowWindow = 0
        kwargs.update(startupinfo=si, creationflags=subprocess.CREATE_NO_WINDOW)
    results = {}
    with (args.out / 'game.log').open('w', encoding='utf-8') as log:
        p = subprocess.Popen([str(args.exe.resolve()), str(args.rom.resolve()),
                              '--hidden', '--no-save', '--tcp', str(args.port),
                              '--config', str(config.resolve()),
                              '--world-view', 'follow', '--world-size', '768x480'],
                             cwd=args.exe.parent, env=env, stdout=log,
                             stderr=subprocess.STDOUT, **kwargs)
        stream = None
        try:
            deadline = time.monotonic() + 15
            while time.monotonic() < deadline:
                if p.poll() is not None:
                    raise RuntimeError(f'game exited {p.returncode}; see game.log')
                try:
                    sock = socket.create_connection(('127.0.0.1', args.port), timeout=1)
                    sock.settimeout(15)
                    stream = sock.makefile('rwb')
                    break
                except OSError:
                    time.sleep(.1)
            if stream is None:
                raise RuntimeError('debug server did not start')
            def cmd(name, **fields):
                stream.write((json.dumps(dict(cmd=name, id=1, **fields))+'\n').encode())
                stream.flush()
                reply = json.loads(stream.readline())
                if not reply.get('ok'):
                    raise RuntimeError(reply)
                return reply
            cmd('menu', open=True)
            cmd('load_state', path=str(args.state.resolve()))
            results['guest'] = cmd('zelda_state')
            results['probe'] = cmd('zelda_world_probe')
            results['picture'] = cmd('screenshot', layer='picture', path=str((args.out/'follow.png').resolve()))
            results['world'] = cmd('zelda_world')
            cmd('quit')
            p.wait(timeout=10)
            assert p.returncode == 0, p.returncode
            assert all(results['probe'][k] for k in ('decoded', 'live_tiles_exact',
                        'live_attrs_exact', 'machine_state_unchanged')), results['probe']
            assert results['picture']['size'] == [768, 480], results['picture']
            assert results['world']['decoded'] >= 128 and results['world']['failures'] == 0
            results['status'] = 'pass'
        finally:
            if stream:
                stream.close()
            if p.poll() is None:
                p.terminate()
                p.wait(timeout=10)
            (args.out/'results.json').write_text(json.dumps(results, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(results))


if __name__ == '__main__':
    main()
