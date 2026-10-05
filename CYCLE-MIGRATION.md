# Cycle CPU migration

The default CMake backend is `cycle`. `-DNESRECOMP_BACKEND=legacy` retains
the former backend in a separate build directory. The cycle executable uses
stock USA PRG0, payload CRC32 `3fe272fb`, for every presentation mode. No ROM
or third-party HD art is distributed.

```sh
cmake -S . -B build-cycle -DNESRECOMP_ROM="/path/to/Legend of Zelda.NES"
cmake --build build-cycle --config Release
python tools/import_hdpack.py --pack "/path/to/pack" --rom "/path/to/Legend of Zelda.NES" --out "Zelda-Remastered.nesmod"
```

Install the local archive through the launcher's **Mods** page, then enable
**Zelda Remastered HD** before Play. NESRecomp's shared HD-pack Mod runtime
verifies the original ROM, pack assets and optional IPS, applies the patch to
an in-memory cartridge copy before power-on, and presents the pack through
the real PPU's recorded fetch/pixel pipeline. The pack's creator notice is
preserved. A separate patched ROM or HD executable is no longer needed for
cycle builds. The Remastered payload remains `fd9c577f`; changed PRG safely
uses the cycle interpreter. Disabling the Mod restores stock native execution
on the next launch. Existing legacy targets remain available explicitly.

The stock cartridge uses its verified SNROM board's 8 KiB battery memory.
Both stock and Remastered accept raw 8 KiB battery files; use the launcher
SAVE import for older `.srm` progress. Normal windowed saves live in
`saves/<ROM stem>.sav` beside the executable. Legacy binary save states do
not transfer between CPU backends. Cycle save/load uses F8/F9 and `.cycstate`;
`--no-save` disables battery loading and writes. Changing cartridge metadata,
pack assets, patch or presentation options invalidates incompatible states
before gameplay changes. Branch states from earlier builds may be incompatible.

The existing Voxel 3D and first-person packages are disabled by default.
These modes and HD share `display-mode`: enabling one in the launcher disables
the others. HD and conflicting display choices are read-only while the game
runs; return to the launcher to change them and restart. Camera controls,
camera-relative movement, room-transition geometry, collision-grid textures
and the centered HUD use a cycle presentation adapter. Cached geometry,
camera and input state are serialized with validation before load. Native 2D
remains 256x240 and voxel views 426x240; no new adaptive 2D widescreen was added.

The real Remastered pack is scale 2 (512x480), with its existing unsupported
condition and missing-image warnings preserved. Replacement music and sound
files remain unsupported; guest audio runs on the NES APU. Paused HD frames
are cached, condition caches are refreshed after a state load, and background
scroll offsets are computed once per frame. HD rendering has existing
performance limits; brief desktop throughput measurements are not a full
performance audit.

Developer override: `--voxel stock|diorama|first-person`. Headless HD checks
use an installed, enabled catalog supplied through `--mods-root`. TCP
`zelda_state`, `entity_snapshot` and `entity_slot` preserve the old diagnostic
fields and add cycle/view details.

Branch validation covers cold startup and registration, an overworld route
with transitions, native/interpreter equality, independent original-machine
oracle comparison on all four NTSC alignments, fresh-process state continuation,
SDL pictures and atomic refusal of incompatible enhancement/pack states.
The shared HD adapter has CHR RAM/banked ROM, scrolling, 8x16 sprite, priority,
flip, emphasis and exact original-pixel fallback fixtures. The real package
is installed through the actual Mods provider; the original ROM is unchanged.
These checks do not establish a new full-game audit.

Run the maintained regression (Pillow required for optional HD checks):

```sh
python tools/cycle_probe.py --stock-exe build-cycle/Release/LegendOfZeldaNESRecomp.exe --stock-rom "/path/to/Legend of Zelda.NES" --hd-mods "/path/to/installed-enabled/mods" --out cycle-evidence/regression
```

The owner briefly played stock, diorama, first-person and the earlier separate
Remastered build on 2026-10-04 and reported “all four pass.” The owner then
requested shared modern HD Mods and adoption work for other games. The updated
single-executable HD Mod was also briefly played on 2026-10-04; the owner
reported “Pass — plays correctly.” All required Zelda preview playtests pass.
Source changes remain on feature branches; no merge or publication has occurred.
