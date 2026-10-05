# Zelda Remastered attribution and legacy patch helper

Cycle builds use the shared modern HD-pack Mods runtime in the stock executable.
Run `python tools/import_hdpack.py --pack <folder> --rom <stock-rom> --out
Zelda-Remastered.nesmod`, then install and enable it through **Mods**. The
framework verifies the pack's IPS and applies it in memory; the original ROM
stays untouched. This directory preserves the creator notice and the older
IPS helper's input for explicit legacy builds and independent comparisons.

## What's here

- `ZeldaHD.ips` — the patch declared by the *Zelda: Remastered* HD pack. It adds
  the detailed/HD heart CHR, re-scripted text, audio hooks, and palette tweaks
  the pack was authored against.
- `ZeldaRemasteredReadme.txt` — the pack authors' readme, kept here as the
  required **license + attribution** (see the `#Legal#` and `#Credits#`
  sections). The pack is freeware for **non-commercial / personal use**, may be
  redistributed *with this notice*, and is the work of **Aclectico, KYA**,
  ShadowOne333 (Zelda 1 Redux), Snarfblam (⅛-heart code), the artists/fonts
  credited within, and **Sour** (Mesen). The underlying game is © Nintendo.

## What is NOT here (you supply it)

- **The ROM** — supply your own legitimate North-American *The Legend of Zelda*
  PRG0 ROM (clean SHA-1 `DAB79C84934F9AA5DB4E7DAD390E5D0C12443FA2`). Never
  bundled.
- **The HD texture pack** — supply *Zelda: Remastered* yourself and import its
  folder with `tools/import_hdpack.py`. Its art is not bundled in the game.

## Explicit legacy HD build

The older legacy executable compiles patched opcodes into C at build time.
Its patch is applied to a derivative **at regen**. These directions apply
only to `NESRECOMP_BACKEND=legacy`:

```
python tools/apply_hd_patch.py --rom <your zelda.nes> --out build/zelda_hd.nes
NESRecomp.exe build/zelda_hd.nes --game game.toml      # regen from the patched ROM
cmake -S . -B build-legacy -DNESRECOMP_BACKEND=legacy
cmake --build build-legacy --target LegendOfZeldaNESRecomp-HD
```

The recompiled code is baked from the patched ROM and the runner reads PRG data
from the loaded ROM at runtime, so the HD build **expects the patched ROM**
(`extras.c` `game_get_expected_crc32` is the patched CRC `0xFD9C577F`). Drop the
HD pack into `hdpack/` and the launcher's HD toggle (default-on) will load it.
