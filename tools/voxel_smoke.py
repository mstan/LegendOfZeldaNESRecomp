#!/usr/bin/env python3
"""Headless Voxel 3D smoke for a built or packaged LegendOfZeldaNESRecomp.

Each presentation mode (stock, voxel overworld, voxel first person) runs in its
own fresh directory: a copy of the package, an empty saves/ (a leftover SRAM
file changes the title-menu flow and desyncs every script), its own
mods/state.toml and its own screenshot directory (NESRECOMP_SHOT_DIR). Modes run
in parallel; scripts within a mode run in order because later scripts load the
states earlier ones save.

A run fails on: nonzero exit (the runner exits 3 when a WAIT_* times out or an
ASSERT_RAM8 fails), any dispatch miss, the wrong voxel mode reporting enabled,
a requested screenshot that was never written, or a movement/camera script
whose frames are all identical.

No window is opened (SDL dummy video/audio, launcher bypassed).

Usage:
  python tools/voxel_smoke.py --rom "Zelda # NES.NES"
      [--package release/LegendOfZeldaNESRecomp-windows-x64.zip | build_release]
      [--out build/voxel_smoke] [--modes stock,diorama,first-person]
"""
import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
EXE = 'LegendOfZeldaNESRecomp.exe'

# Scripts that move Link or the camera between shots must show change; one
# frame for every shot means the game was paused or input never arrived.
CHANGES = {'voxel_transition_capture.script', 'voxel_door_capture.script',
           'voxel_numpad_capture.script', 'voxel_sprite_angle_capture.script',
           'voxel_first_person_camera_relative_capture.script'}

MODES = {
    'stock': {
        'feature': None,
        'scripts': ['voxel_capture.script', 'voxel_transition_capture.script',
                    'voxel_door_capture.script'],
    },
    'diorama': {
        'feature': ('legend-of-zelda.enhancement.voxel-diorama', 'voxel-diorama',
                    'Zelda overworld diorama enabled'),
        'scripts': ['voxel_capture.script', 'voxel_camera_capture.script',
                    'voxel_transition_capture.script', 'voxel_door_capture.script',
                    'voxel_numpad_capture.script', 'voxel_sprite_angle_capture.script',
                    'voxel_tektite_state.script', 'voxel_tektite_grouping_capture.script'],
    },
    'first-person': {
        'feature': ('legend-of-zelda.enhancement.voxel-first-person', 'voxel-first-person',
                    'Zelda first-person enabled'),
        'scripts': ['voxel_capture.script',
                    'voxel_first_person_camera_relative_capture.script',
                    'voxel_first_person_input_release.script',
                    'voxel_transition_capture.script'],
    },
}


def stage(package: Path, dest: Path, rom: Path, feature):
    if dest.exists():
        shutil.rmtree(dest)
    if package.is_file():
        with zipfile.ZipFile(package) as z:
            for info in z.infolist():
                # Compress-Archive (PS 5.1) writes backslash separators.
                name = info.filename.replace('\\', '/')
                target = dest / name
                if name.endswith('/'):
                    target.mkdir(parents=True, exist_ok=True)
                    continue
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(z.read(info))
    else:
        shutil.copytree(package, dest, ignore=shutil.ignore_patterns(
            'saves', 'savestates', 'state.toml', 'debug.ini', 'dispatch_misses.log',
            'CMakeFiles', '*.obj', '*.pdb', '*.ilk', '.ninja*', 'CMakeCache.txt'))
    if not (dest / EXE).exists():
        raise SystemExit(f'{EXE} missing from {package}')
    for leftover in ('saves', 'savestates'):
        shutil.rmtree(dest / leftover, ignore_errors=True)
    for leftover in ('debug.ini', 'dispatch_misses.log', 'mods/state.toml'):
        (dest / leftover).unlink(missing_ok=True)
    shutil.copyfile(rom, dest / 'zelda.nes')
    (dest / 'tests').mkdir(exist_ok=True)
    if feature:
        package_id, feature_id, _ = feature
        (dest / 'mods').mkdir(exist_ok=True)
        (dest / 'mods' / 'state.toml').write_text(
            'format_version = 1\n[[package]]\n'
            f'id = "{package_id}"\nversion = "1.0.0"\n'
            f'[[feature]]\npackage_id = "{package_id}"\nid = "{feature_id}"\nenabled = true\n',
            encoding='utf-8')


def run_script(run_dir: Path, script: str, timeout: int):
    src = ROOT / 'tests' / script
    shutil.copyfile(src, run_dir / script)
    shots = run_dir / 'shots' / Path(script).stem
    shots.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy',
               NESRECOMP_NO_LAUNCHER='1', NESRECOMP_SHOT_DIR=str(shots))
    try:
        p = subprocess.run([str(run_dir / EXE), 'zelda.nes', '--script', script],
                           cwd=run_dir, env=env, capture_output=True, text=True,
                           errors='replace', timeout=timeout)
        code, out = p.returncode, p.stdout + p.stderr
    except subprocess.TimeoutExpired as e:
        code = 'timeout'
        out = (e.stdout or b'').decode(errors='replace') + (e.stderr or b'').decode(errors='replace')
    (run_dir / f'{Path(script).stem}.log').write_text(out, encoding='utf-8')
    return code, out, shots


def run_mode(name: str, package: Path, rom: Path, out_root: Path, timeout: int):
    mode = MODES[name]
    run_dir = out_root / name
    stage(package, run_dir, rom, mode['feature'])
    results = []
    for script in mode['scripts']:
        code, out, shots = run_script(run_dir, script, timeout)
        problems = []
        if code != 0:
            problems.append(f'exit {code}')
        problems += [l.strip() for l in out.splitlines()
                     if re.search(r'TIMEOUT|ASSERT FAIL|\[Script\] FAILED', l)]
        if mode['feature']:
            if mode['feature'][2] not in out:
                problems.append(f'"{mode["feature"][2]}" not reported')
        elif '[Voxel]' in out and 'enabled' in out:
            problems.append('voxel enabled in stock run')
        requested = re.findall(r'\[Script\] SCREENSHOT -> (\S+)', out)
        written = sorted(p for p in shots.glob('*.png'))
        if len(written) != len(requested):
            problems.append(f'{len(requested)} screenshots requested, {len(written)} written')
        hashes = {hashlib.sha1(p.read_bytes()).hexdigest() for p in written}
        if script in CHANGES and len(written) > 1 and len(hashes) == 1:
            problems.append('all frames identical (input had no effect)')
        results.append({'script': script, 'exit': code, 'shots': len(written),
                        'distinct': len(hashes), 'problems': problems})
        if code != 0:
            break  # later scripts depend on this one's saved state
    misses = run_dir / 'dispatch_misses.log'
    miss_lines = misses.read_text(errors='replace').splitlines() if misses.exists() else []
    return {'mode': name, 'dir': str(run_dir), 'dispatch_misses': len(miss_lines),
            'scripts': results,
            'ok': not miss_lines and all(not r['problems'] for r in results)
                  and len(results) == len(mode['scripts'])}


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--rom', required=True, type=Path)
    ap.add_argument('--package', type=Path,
                    default=ROOT / 'release' / 'LegendOfZeldaNESRecomp-windows-x64.zip')
    ap.add_argument('--out', type=Path, default=ROOT / 'build' / 'voxel_smoke')
    ap.add_argument('--modes', default=','.join(MODES))
    ap.add_argument('--timeout', type=int, default=600, help='seconds per script')
    a = ap.parse_args()
    names = [m for m in a.modes.split(',') if m]
    unknown = [m for m in names if m not in MODES]
    if unknown:
        raise SystemExit(f'unknown mode(s): {", ".join(unknown)}')
    a.out.mkdir(parents=True, exist_ok=True)
    with concurrent.futures.ThreadPoolExecutor(len(names)) as pool:
        reports = list(pool.map(lambda m: run_mode(m, a.package.resolve(), a.rom.resolve(),
                                                   a.out.resolve(), a.timeout), names))
    (a.out / 'report.json').write_text(json.dumps(reports, indent=2), encoding='utf-8')
    for r in reports:
        print(f"{'PASS' if r['ok'] else 'FAIL'}  {r['mode']}  misses={r['dispatch_misses']}")
        for s in r['scripts']:
            flag = 'ok ' if not s['problems'] else 'BAD'
            print(f"   {flag} {s['script']}  shots={s['shots']} distinct={s['distinct']}"
                  + (f"  {'; '.join(s['problems'])}" if s['problems'] else ''))
    sys.exit(0 if all(r['ok'] for r in reports) else 1)


if __name__ == '__main__':
    main()
