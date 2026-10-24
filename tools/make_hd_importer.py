#!/usr/bin/env python3
"""Package the complete standalone HD importer, without game or pack assets."""
import argparse
from pathlib import Path
import zipfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    root = Path(__file__).resolve().parents[1]
    parser.add_argument('--framework', type=Path, default=root / 'nesrecomp')
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    files = {
        'tools/import_hdpack.py': root / 'tools/import_hdpack.py',
        'tools/package_hdpack.py': args.framework / 'tools/package_hdpack.py',
        'GENERIC-IMPORTER-README.txt': args.framework / 'tools/HD-IMPORTER-README.txt',
        'hdpatch/ZeldaRemasteredReadme.txt': root / 'hdpatch/ZeldaRemasteredReadme.txt',
        'README.txt': root / 'tools/HD-IMPORTER-README.txt',
        'LICENSE': root / 'LICENSE',
    }
    for path in files.values():
        if not path.is_file():
            parser.error(f'Missing required importer file: {path}')
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(args.out, 'x', compression=zipfile.ZIP_DEFLATED) as archive:
        for name, path in files.items():
            archive.write(path, name)
    print(f'Created {args.out}: complete importer, no ROM or HD assets')


if __name__ == '__main__':
    main()
