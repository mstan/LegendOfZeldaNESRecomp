# Cycle CPU migration

The default CMake backend is `cycle`. `-DNESRECOMP_BACKEND=legacy` retains
the former backend in a separate build directory. Stock USA PRG0 targets
headerless PRG+CHR CRC32 `3fe272fb`; the optional local Remastered derivative
targets `fd9c577f`. No ROM or third-party HD assets are distributed.

```sh
cmake -S . -B build-cycle -DNESRECOMP_ROM="/path/to/Legend of Zelda.NES"
cmake --build build-cycle --config Release
python tools/apply_hd_patch.py --rom "/path/to/Legend of Zelda.NES" --out build-cycle/zelda_hd.nes
cmake -S . -B build-cycle -DZELDA_HD_ROM="/absolute/path/build-cycle/zelda_hd.nes"
cmake --build build-cycle --config Release
```

The patch helper records the verified SNROM board's 8 KiB battery memory in
the derivative's header. Original files stay unchanged. Both cycle variants
accept raw 8 KiB battery files; use the launcher SAVE import for older `.srm`
progress. Normal windowed saves live in `saves/<ROM stem>.sav` beside the
executable. Legacy binary save states cannot transfer between CPU backends.
Cycle save/load uses F8/F9 and `.cycstate`; `--no-save` disables battery loading
and writes. Changing cartridge metadata invalidates earlier branch states.

Stock includes the existing mutually exclusive Voxel 3D and first-person
packages, disabled by default. Camera controls, camera-relative movement,
room-transition geometry, live collision-grid textures and the centered HUD
use a cycle presentation adapter. The cached geometry, camera and input state
are serialized with validation before load. Native 2D remains 256x240; the
voxel views remain 426x240. This is the existing voxel presentation, without
a new adaptive 2D widescreen implementation.

Remastered uses the existing HD texture/background sampler on the real PPU
pipeline. Point the launcher at a local folder containing `hires.txt`; the
original pack can also be placed in `hdpack/` beside the HD executable.
The tested pack is scale 2 (512x480), with existing unsupported-condition and
missing-image warnings preserved. Replacement music/sound files remain
unsupported; guest audio still runs on the NES APU. Stock-only voxel packages
do not activate for the patched ROM.

Developer overrides: `--voxel stock|diorama|first-person`, and, on the HD
target, `--hdpack <folder>|off`. TCP `zelda_state`, `entity_snapshot` and
`entity_slot` preserve the old diagnostic fields and add cycle/view details.

Branch validation covers cold startup and registration, a short overworld
route with transitions, native/interpreter equality, original-machine oracle
comparison on all four NTSC alignments, fresh-process state continuation,
saved SDL pictures and atomic refusal of incompatible enhancement states.
The HD adapter also has framework fixtures for CHR RAM/banked ROM, scrolling,
8x16 sprites, priority, flips, emphasis and exact original pixel fallback.
The explicit legacy stock and HD targets compile using their existing
generated inputs. These checks do not establish a new full-game audit.

Run the maintained game regression (Pillow needed for the optional HD checks):

```sh
python tools/cycle_probe.py --stock-exe build-cycle/Release/LegendOfZeldaNESRecomp.exe --stock-rom "/path/to/Legend of Zelda.NES" --hd-exe build-cycle/Release/LegendOfZeldaNESRecomp-HD.exe --hd-rom build-cycle/zelda_hd.nes --hdpack "/path/to/pack" --out cycle-evidence/regression
```

Owner playtests for stock, diorama, first-person and Remastered are required
before integration. Source changes remain on feature branches; no merge or
publication has occurred.
