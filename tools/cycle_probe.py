"""Zelda cycle regression: real inputs, native/interpreter and fresh-process replay.

Provide your verified stock ROM and optional installed, enabled Remastered Mods catalog.
Evidence stays in --out; all runs disable battery saves and hide native windows.
The optional HD pixel comparisons require Pillow.
"""
import argparse
import json
from pathlib import Path
import subprocess

ROUTE = ('0 -\n360 START\n362 -\n482 START\n484 -\n650 A\n658 -\n'
         '666 SELECT\n674 -\n682 SELECT\n690 -\n698 SELECT\n706 -\n'
         '766 START\n774 -\n1014 START\n1016 -\n1136 START\n1138 -\n'
         '1400 START\n1408 -\n1800 UP\n1860 -\n2100 DOWN\n2220 -\n'
         '2350 RIGHT A\n2500 -\n2700 UP A\n2850 -\n')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    for name in ('stock-exe', 'stock-rom', 'out'):
        ap.add_argument('--' + name, type=Path, required=True)
    for name in ('hd-mods', 'interp'):
        ap.add_argument('--' + name, type=Path)
    args = ap.parse_args()
    out = args.out.resolve(); out.mkdir(parents=True, exist_ok=True)
    route = out / 'route.txt'; route.write_text(ROUTE)
    startup = None
    if hasattr(subprocess, 'STARTUPINFO'):
        startup = subprocess.STARTUPINFO()
        startup.dwFlags |= subprocess.STARTF_USESHOWWINDOW; startup.wShowWindow = 0

    def run(exe, rom, name, options):
        with (out / (name + '.log')).open('w') as log:
            p = subprocess.run([str(exe.resolve()), str(rom.resolve()), '--no-save',
                                '--frames', '3000', '--input', str(route), *map(str, options)],
                               cwd=out, stdout=log, stderr=subprocess.STDOUT, startupinfo=startup,
                               creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0), timeout=600)
        if p.returncode:
            raise RuntimeError((out / (name + '.log')).read_text(errors='replace')[-3000:])

    cases = [(name, args.stock_exe, args.stock_rom, ['--voxel', name])
             for name in ('stock', 'diorama', 'first-person')]
    if args.hd_mods:
        from PIL import Image
        cases.append(('hd-textures', args.stock_exe, args.stock_rom,
                      ['--mods-root', args.hd_mods.resolve()]))
    results = []
    for tag, exe, rom, options in cases:
        def artifacts(name):
            return ['--hash-out', out/(name+'.hash'), '--present-out', out/(name+'.png'),
                    '--screenshot', out/(name+'-native.png'),
                    '--save-state', f'2999:{out/(name+"-final.cycstate")}']
        for mode in ('native', 'interp'):
            name = tag + '-' + mode
            run(exe, rom, name, [*artifacts(name), '--save-state', f'2699:{out/(name+"-middle.cycstate")}',
                                *options, *(['--interp-only'] if mode == 'interp' else [])])
        for suffix in ('.hash', '.png', '-native.png', '-middle.cycstate', '-final.cycstate'):
            assert (out/(tag+'-native'+suffix)).read_bytes() == (out/(tag+'-interp'+suffix)).read_bytes(), (tag,suffix)
        replay = tag + '-replay'
        run(exe, rom, replay, [*artifacts(replay), '--load-state', out/(tag+'-native-middle.cycstate'), *options])
        assert (out/(replay+'.hash')).read_text().splitlines() == (out/(tag+'-native.hash')).read_text().splitlines()[-300:]
        for suffix in ('.png', '-native.png', '-final.cycstate'):
            assert (out/(replay+suffix)).read_bytes() == (out/(tag+'-native'+suffix)).read_bytes(), (tag,'replay',suffix)
        if tag == 'hd-textures':
            actual = Image.open(out/(tag+'-native.png')).convert('RGBA')
            native = Image.open(out/(tag+'-native-native.png')).convert('RGBA')
            assert actual.size == (512,480)
            same = actual.tobytes() == native.resize(actual.size, Image.Resampling.NEAREST).tobytes()
            assert not same, (tag,'HD artwork')
        results.append({'case':tag, 'frames':3000, 'native_interpreter':'exact', 'replay_frames':300})
        print(tag, 'native/interpreter and replay passed', flush=True)
    assert (out/'stock-native.hash').read_bytes() == (out/'diorama-native.hash').read_bytes()
    if args.interp:
        run(args.interp, args.stock_rom, 'standalone', ['--hash-out', out/'standalone.hash'])
        assert (out/'standalone.hash').read_bytes() == (out/'stock-native.hash').read_bytes()
    (out/'results.json').write_text(json.dumps(results, indent=2))


if __name__ == '__main__':
    main()
