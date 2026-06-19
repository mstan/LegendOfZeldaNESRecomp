# Zelda HD build (EXPERIMENTAL)

This directory holds the IPS patch for building the **HD ("Zelda Remastered")**
variant. HD texture-pack support in the recompiler is **experimental**.

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
- **The HD texture pack** — download *Zelda: Remastered* yourself and drop its
  contents (the `hires.txt` + PNGs) into the build's `hdpack/` folder (or point
  the launcher's *HD Texture Pack* picker at it). Not bundled.

## How the HD build works

The recompiler is a *static* recompiler: the ROM's code is translated to C at
build time, so the patch is applied **at regen**, not at runtime:

```
python tools/apply_hd_patch.py --rom <your zelda.nes> --out build/zelda_hd.nes
NESRecomp.exe build/zelda_hd.nes --game game.toml      # regen from the patched ROM
cmake --build build_release
```

The recompiled code is baked from the patched ROM and the runner reads PRG data
from the loaded ROM at runtime, so the HD build **expects the patched ROM**
(`extras.c` `game_get_expected_crc32` is the patched CRC `0xFD9C577F`). Drop the
HD pack into `hdpack/` and the launcher's HD toggle (default-on) will load it.
