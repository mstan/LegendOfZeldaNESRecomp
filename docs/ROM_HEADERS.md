# ROM header compatibility

The launcher verifies the original PRG+CHR game data. The runtime also checks
the cartridge board needed by native code. Equivalent legacy iNES and NES 2.0
headers are accepted after resolving known legacy board details. NTSC and PAL
profiles remain separate; different game data, board wiring, RAM geometry or
console metadata cannot bypass these checks.

No ROM is included. USA retail images remain preferred where available.
This patch retains the existing renderer and enhancement packages. Automated
release checks cover header acceptance, startup and native/interpreter
execution; they do not constitute a new complete stage/mechanics audit.

## Cartridge profiles

`ZELDA_ROM_VARIANT` (CMake cache) selects the recompiler profile under
`config/cycle/`, and `NESRECOMP_ROM` supplies the image the code is generated
from. The profile only describes how the code is discovered; the image identity
is hashed from the file at configure time (`CYC_PROJECT_ROM_SHA256`,
`CYC_PROJECT_LAUNCHER_SHA256`, `CYC_PROJECT_MOD_CRC32`).

Which images the executable accepts is not derived from the files a machine has:
`config/cartridges/accepted-images.txt` lists each image by payload CRC32, PRG FNV-1a
and payload SHA-256, and configure bakes that list into
`cyc_game_image_accepted()` and the launcher allowlist. Cartridge data itself is
never part of this repository. An image that is not the compiled one runs on the
cycle-accurate interpreter; the README explains why.

| Variant | Image | PRG payload CRC32 |
|---------|-------|-------------------|
| `stock` (default) | USA PRG0 retail | `3fe272fb` |
| `relocalized` | "Legend of Zelda, The (USA) [T-Eng by SimpleDanny v1.1] [Add by Snarfblam v0.2] [Font mod] [Relocalized]" | `397b56a6` |

### `relocalized`

The image is 8 x 16 KB PRG with CHR RAM (mapper 1, MMC1), the same board shape
as the retail image: `4E 45 53 1A 08 00 12 08 00 00 70 07 00 00 00 01`,
payload SHA-256
`5a8e3ab3e0b198ba9aba9068f7eef44e9e2cbe6fefd4ee27faa51dec5356605e` after the
16-byte iNES header. Vectors are NMI `$E484`, RESET `$FF50`, IRQ `$FFF0`.

The translation, added content and modified font live in cartridge data banks;
the executable layout is unchanged, so `config/cycle/game-cycle-relocalized.toml`
carries the same discovery inputs as `stock`:

- the seven `[[mod_function_hook]]` six-byte signatures all match, and the
  recompiler's `audit_mod_function_hooks` reports no unmatched entry;
- the `[[sram_map]]` startup copy (bank 1, `$A500` -> `$6C90`, `0x1370` bytes)
  applies unchanged;
- the interpreter seed list derived from the retail image still names
  instruction starts in this image; codegen reports `8815 instructions
  compiled`, all but 11 executed natively at runtime (99.9%).

The preloaded enhancement packages name the images they run on
(`[[target]] rom_crc32`), and their world decoder re-checks the image itself
(`zelda_world_supported()` in `src/zelda_world.c`). The build stages only the
packages whose declared identity equals the payload CRC32 it computes from
`NESRECOMP_ROM`; a package that targets another image is reported at configure
time and skipped, because a default-enabled feature it cannot serve would
surface as an unresolvable Mods conflict and block PLAY.

The experimental packs (`seamless-world`, `voxel-diorama`,
`voxel-first-person`) now declare `3fe272fb`, `c452304e` and `397b56a6`, and
`zelda_world_supported()` accepts those three payload CRCs. The gate exists
because the decoder is not a reimplementation: `decode_to()` enters *this
image's own* routines on an isolated machine — `SwitchBank` at `$FFAC`, then
`$A8C4` and `$83D7` in bank 5 — and reads back the room they built. Accepting
an image therefore means proving those entries still hold the expected code.
For `c452304e` and `397b56a6` that was checked structurally: `$83D7` is a
compiled instruction label (`L_83D7`, opcode `A8 TAY`) in the generated bank
code, `$A8C4` and `$FFAC` are jump targets there, and the `$A8C4`/`$83D7`
bytes are identical in both dumps. `zelda_world_probe` then reports
`decoded:true` and leaves the machine state unchanged.

That is a host-level check, not a gameplay one: the probe's live comparison
(`live_tiles_exact`) needs GameMode 5 with a room loaded, and the automated
input route stops at the title/file-select/name-entry screens. So the
experimental visual output on these images is enabled and toggleable but has
not been verified by eye.

### Another USA dump: `c452304e`

A second USA dump
(`Legend of Zelda, The (USA).nes`, payload CRC32 `c452304e`, payload SHA-256
`8e41e52e11ee5901c68ae5643463b3fe33b4a90665383d2babc622e993f30ee5`) builds
under the `stock` profile. It differs from the `relocalized` image in 5 250
bytes, all inside data-heavy banks (1, 2, 6, plus 19 and 38 bytes in banks 3
and 7), and its executable layout matches the profile's discovery inputs: every
`[[mod_function_hook]]` signature matched, codegen reported 8 807 instructions
with no warning, and 1 800 frames of native execution were byte-identical to
`--interp-only`. A build from this image rejects the `relocalized` image (and
the reverse) at the cartridge-metadata check.

Verification used for this profile (`--hash-out` files compared frame by frame,
`hw=` excluded, the same comparison `tools/cyc/cyc_verify.py` makes between a
native build and its `--interp-only` run):

- 1800 frames of boot/attract: identical memory, picture, registers and cycle
  counts, and identical final PNG;
- 2500 frames driven by an `--input` schedule (title -> file select -> first
  playable scene, movement and item use): identical for every frame and
  identical final PNG.
