#!/usr/bin/env python3
"""Create a local Zelda Remastered .nesmod from the owner's stock ROM and pack."""
import argparse
from pathlib import Path
import subprocess
import sys

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pack', type=Path, required=True)
    parser.add_argument('--rom', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--framework', type=Path, default=Path(__file__).resolve().parents[1]/'nesrecomp')
    args = parser.parse_args()
    here = Path(__file__).resolve().parent
    generator = here/'package_hdpack.py'
    if not generator.is_file():
        generator = args.framework/'tools/package_hdpack.py'
    startup = None
    if hasattr(subprocess, 'STARTUPINFO'):
        startup = subprocess.STARTUPINFO()
        startup.dwFlags |= subprocess.STARTF_USESHOWWINDOW; startup.wShowWindow = 0
    return subprocess.call([sys.executable, str(generator), '--pack', str(args.pack),
        '--rom', str(args.rom), '--game-id', 'legend-of-zelda-us',
        '--id', 'legend-of-zelda.remastered', '--name', 'Zelda Remastered HD',
        '--author', 'Aclectico, KYA and contributors (see included credits)',
        '--license', 'Original pack terms: noncommercial personal use; preserve included notice',
        '--notice', str(here.parent/'hdpatch/ZeldaRemasteredReadme.txt'), '--out', str(args.out)],
        startupinfo=startup, creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0))

if __name__ == '__main__':
    raise SystemExit(main())
