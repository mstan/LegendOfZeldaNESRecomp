"""Built-game save/load regression, including inactive voxel history.

Use a disposable output directory; battery saves and the launcher are disabled.
Pass --input tools/cycle_probe.py's recorded route and --frame 2400 to exercise
gameplay rather than the default title-screen checkpoint.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess


def records(blob):
    result = []
    at = 32
    while at < len(blob):
        tag, size = struct.unpack_from('<4sI', blob, at)
        at += 8
        payload = blob[at:at + size]
        assert len(payload) == size
        result.append((tag, payload))
        at += size
    assert at == len(blob)
    return result


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    for name in ('exe', 'rom', 'out'):
        ap.add_argument('--' + name, type=Path, required=True)
    ap.add_argument('--input', type=Path)
    ap.add_argument('--frame', type=int, default=180)
    a = ap.parse_args()
    a.out = a.out.resolve()
    a.out.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, NESRECOMP_NO_LAUNCHER='1', SDL_AUDIODRIVER='dummy',
               SDL_VIDEODRIVER='dummy')
    common = [str(a.exe.resolve()), str(a.rom.resolve()), '--no-save']
    if a.input:
        common += ['--input', str(a.input.resolve())]
    profiles = dict(stock=[], diorama=['--voxel', 'diorama'],
                    first_person=['--voxel', 'first-person'],
                    seamless=['--world-view', 'seamless', '--world-size', '768x480'])
    results = {}

    def run(name, options, accepted=True):
        p = subprocess.run(common + list(map(str, options)), cwd=a.out, env=env,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                           creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0),
                           timeout=180)
        log = p.stdout.decode(errors='replace')
        (a.out / (name + '.log')).write_text(log, encoding='utf-8')
        assert p.returncode == (0 if accepted else 2), (name, p.returncode, log[-1500:])
        if not accepted:
            assert "mod state 'zelda.cycle.voxel' is incompatible; nothing loaded" in log, log

    cases = [(name, args, args) for name, args in profiles.items()]
    cases += [(name + '_after_first_person', ['--voxel', 'first-person', '--voxel', 'stock', *profiles[name]], profiles[name])
              for name in ('stock', 'seamless')]
    for name, before, after in cases:
        middle = a.out / (name + '.state')
        full = a.out / (name + '-full.state')
        replay = a.out / (name + '-replay.state')
        end = a.frame + 24
        run(name + '-save', [*before, '--frames', end + 1,
                            '--save-state', f'{a.frame}:{middle}',
                            '--save-state', f'{end}:{full}',
                            '--present-out', a.out / (name + '-full.png')])
        run(name + '-load', [*after, '--frames', end + 1, '--load-state', middle,
                            '--save-state', f'{end}:{replay}',
                            '--present-out', a.out / (name + '-replay.png')])
        # TRAC describes the execution path; the machine and every mod record
        # must agree after executing the same remaining inputs in a fresh process.
        left = [(t, p) for t, p in records(full.read_bytes()) if t != b'TRAC']
        right = [(t, p) for t, p in records(replay.read_bytes()) if t != b'TRAC']
        assert left == right, (name, 'replayed architectural state differs')
        assert (a.out / (name + '-full.png')).read_bytes() == (a.out / (name + '-replay.png')).read_bytes(), (name, 'presentation differs')
        results[name] = dict(load=True, replay_machine_and_mods_exact=True, picture_exact=True)
        print(name + ': save/load, replay and picture passed', flush=True)

    for saved, loaded in [('stock', 'diorama'), ('diorama', 'stock'),
                          ('diorama', 'first_person'), ('first_person', 'diorama')]:
        name = saved + '-to-' + loaded
        run(name, [*profiles[loaded], '--frames', a.frame + 2, '--load-state',
                   a.out / (saved + '.state')], accepted=False)
        results[name] = dict(rejected=True)

    original = (a.out / 'diorama.state').read_bytes()
    for kind in ('version', 'truncated', 'missing', 'empty', 'nan'):
        chunks = []
        for tag, payload in records(original):
            if tag == b'MOD ' and payload[:64].rstrip(b'\0') == b'zelda.cycle.voxel':
                if kind == 'version':
                    payload = payload[:64] + b'\xff' + payload[65:]
                elif kind == 'truncated':
                    payload = payload[:-1]
                elif kind == 'missing':
                    continue
                elif kind == 'empty':
                    payload = payload[:64]
                else:
                    payload = payload[:68] + struct.pack('<f', float('nan')) + payload[72:]
            chunks.append(struct.pack('<4sI', tag, len(payload)) + payload)
        path = a.out / ('invalid-' + kind + '.state')
        path.write_bytes(original[:32] + b''.join(chunks))
        run('invalid-' + kind, [*profiles['diorama'], '--frames', a.frame + 2,
                               '--load-state', path], accepted=False)
        results['invalid-' + kind] = dict(rejected=True)
    results['binary_sha256'] = hashlib.sha256(a.exe.read_bytes()).hexdigest()
    (a.out / 'results.json').write_text(json.dumps(results, indent=2) + '\n')
    print('Mode isolation and invalid-record rejection passed', flush=True)


if __name__ == '__main__':
    main()
