#!/usr/bin/env python3
"""Create a local Zelda Remastered .nesmod from the owner's stock ROM and pack."""
import argparse
from contextlib import contextmanager
from pathlib import Path
from pathlib import PurePosixPath
import shutil
import stat
import subprocess
import sys
import tempfile
import zipfile

MAX_FILES = 4096
MAX_BYTES = 256 * 1024 * 1024


def unpack(archive, destination):
    """Extract a bounded pack without links, path traversal or overwriting files."""
    entries = archive.infolist()
    if len(entries) > MAX_FILES or sum(p.file_size for p in entries) > MAX_BYTES:
        raise ValueError('Pack exceeds .nesmod size/file limits')
    seen = set()
    for entry in entries:
        name = PurePosixPath(entry.filename.replace('\\', '/'))
        mode = entry.external_attr >> 16
        key = name.as_posix().casefold()
        if (name.is_absolute() or '..' in name.parts or ':' in str(name)
                or not name.parts or key in seen or stat.S_ISLNK(mode)
                or any(part.endswith(('.', ' ')) for part in name.parts)):
            raise ValueError(f'Unsafe or duplicate pack path: {entry.filename}')
        seen.add(key)
        target = destination.joinpath(*name.parts)
        if entry.is_dir():
            target.mkdir(parents=True, exist_ok=True)
        else:
            target.parent.mkdir(parents=True, exist_ok=True)
            with archive.open(entry) as source, target.open('xb') as output:
                shutil.copyfileobj(source, output)


@contextmanager
def pack_folder(path):
    """Accept Mesen's inner pack, the original v1.3 download, or an extracted folder."""
    path = path.resolve(strict=True)
    if path.is_dir() and (path / 'hires.txt').is_file():
        yield path
        return
    if path.is_dir():
        path = path / 'ZeldaRemastered.zip'
    if not path.is_file():
        raise ValueError('Select a pack ZIP or a folder containing hires.txt or ZeldaRemastered.zip')
    with tempfile.TemporaryDirectory(prefix='zelda-hd-import-') as scratch:
        temporary = Path(scratch)
        with zipfile.ZipFile(path) as archive:
            if (len(archive.infolist()) > MAX_FILES
                    or sum(p.file_size for p in archive.infolist()) > MAX_BYTES):
                raise ValueError('Pack exceeds .nesmod size/file limits')
            names = archive.namelist()
            if 'hires.txt' in names:
                unpack(archive, temporary / 'pack')
            elif 'ZeldaRemastered.zip' in names:
                # Only extract the main pack; alternate art/audio are optional variants.
                entry = archive.getinfo('ZeldaRemastered.zip')
                if entry.file_size > MAX_BYTES:
                    raise ValueError('Nested pack exceeds .nesmod size limit')
                nested = temporary / 'ZeldaRemastered.zip'
                with archive.open(entry) as source, nested.open('xb') as output:
                    shutil.copyfileobj(source, output)
                with zipfile.ZipFile(nested) as inner:
                    unpack(inner, temporary / 'pack')
            else:
                raise ValueError('ZIP contains neither hires.txt nor ZeldaRemastered.zip')
        folder = temporary / 'pack'
        if not (folder / 'hires.txt').is_file():
            raise ValueError('The main pack is missing hires.txt')
        yield folder

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
    if not generator.is_file():
        parser.error('Missing package_hdpack.py. Extract the complete HD importer download or initialize the framework submodule.')
    startup = None
    if hasattr(subprocess, 'STARTUPINFO'):
        startup = subprocess.STARTUPINFO()
        startup.dwFlags |= subprocess.STARTF_USESHOWWINDOW; startup.wShowWindow = 0
    try:
        with pack_folder(args.pack) as folder:
            return subprocess.call([sys.executable, str(generator), '--pack', str(folder),
                '--rom', str(args.rom), '--game-id', 'legend-of-zelda-us',
                '--id', 'legend-of-zelda.remastered', '--name', 'Zelda Remastered HD',
                '--author', 'Aclectico, KYA and contributors (see included credits)',
                '--license', 'Original pack terms: noncommercial personal use; preserve included notice',
                '--notice', str(here.parent/'hdpatch/ZeldaRemasteredReadme.txt'), '--out', str(args.out)],
                startupinfo=startup, creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0))
    except (OSError, ValueError, zipfile.BadZipFile) as error:
        parser.error(str(error))

if __name__ == '__main__':
    raise SystemExit(main())
