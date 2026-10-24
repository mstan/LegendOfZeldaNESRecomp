"""Archive input regressions; uses synthetic data and never launches the game."""
import importlib.util
import io
from pathlib import Path
import stat
import tempfile
import unittest
import zipfile

spec = importlib.util.spec_from_file_location('import_hdpack', Path(__file__).resolve().parents[1] / 'tools/import_hdpack.py')
importer = importlib.util.module_from_spec(spec)
spec.loader.exec_module(importer)


def archive_bytes(files):
    output = io.BytesIO()
    with zipfile.ZipFile(output, 'w', zipfile.ZIP_DEFLATED) as archive:
        for name, data in files.items():
            archive.writestr(name, data)
    return output.getvalue()


class ImportTests(unittest.TestCase):
    def setUp(self):
        self.scratch = tempfile.TemporaryDirectory()
        self.addCleanup(self.scratch.cleanup)
        self.root = Path(self.scratch.name)
        self.inner = archive_bytes({'hires.txt': '<ver>106\n<scale>2\n', 'art.png': b'fixture'})

    def test_original_download_and_inner_zip_and_directory(self):
        outer = self.root / 'UnZipMeFirstZeldaRemasteredV1_3.zip'
        outer.write_bytes(archive_bytes({'ZeldaRemastered.zip': self.inner, 'ZeldaHDAlternateGraphics/unused.png': b'variant'}))
        inner = self.root / 'ZeldaRemastered.zip'
        inner.write_bytes(self.inner)
        folder = self.root / 'extracted'
        folder.mkdir()
        (folder / 'hires.txt').write_text('<ver>106\n<scale>2\n')
        for source in (outer, inner, self.root, folder):
            with self.subTest(source=source), importer.pack_folder(source) as pack:
                self.assertEqual((pack / 'hires.txt').read_text(), '<ver>106\n<scale>2\n')
                self.assertFalse((pack / 'ZeldaHDAlternateGraphics').exists())
                temporary = pack
            if source != folder:
                self.assertFalse(temporary.exists(), 'temporary extracted assets leaked')
        self.assertEqual(inner.read_bytes(), self.inner)

    def test_unsafe_paths_links_and_duplicate_names_rejected(self):
        for name in ('../escaped', '/absolute', 'C:/absolute', 'dir/../../escaped', 'dir\\..\\escaped', 'trailing. '):
            with self.subTest(name=name):
                source = self.root / 'unsafe.zip'
                source.write_bytes(archive_bytes({'hires.txt': 'fixture', name: 'bad'}))
                with self.assertRaises(ValueError), importer.pack_folder(source):
                    self.fail('unsafe ZIP was accepted')
        for kind in ('link', 'duplicate'):
            source = self.root / 'unsafe.zip'
            with zipfile.ZipFile(source, 'w') as archive:
                archive.writestr('hires.txt', 'fixture')
                if kind == 'link':
                    info = zipfile.ZipInfo('link')
                    info.external_attr = (stat.S_IFLNK | 0o777) << 16
                    archive.writestr(info, '../target')
                else:
                    archive.writestr('HIRES.TXT', 'ambiguous on Windows')
            with self.subTest(kind=kind), self.assertRaises(ValueError), importer.pack_folder(source):
                self.fail('unsafe ZIP was accepted')
        self.assertFalse((self.root.parent / 'escaped').exists())

    def test_size_limit_and_missing_main_pack(self):
        source = self.root / 'pack.zip'
        source.write_bytes(self.inner)
        previous = importer.MAX_BYTES
        try:
            importer.MAX_BYTES = 1
            with self.assertRaisesRegex(ValueError, 'limit'), importer.pack_folder(source):
                self.fail('oversized ZIP was accepted')
        finally:
            importer.MAX_BYTES = previous
        source.write_bytes(archive_bytes({'readme.txt': 'no pack'}))
        with self.assertRaisesRegex(ValueError, 'neither'), importer.pack_folder(source):
            self.fail('ZIP without a main pack was accepted')


if __name__ == '__main__':
    unittest.main()
