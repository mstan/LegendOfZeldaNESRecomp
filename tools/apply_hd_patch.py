#!/usr/bin/env python3
"""Apply the Zelda Remastered HD-pack IPS patch to a stock Zelda ROM, for regen.

The HD ("Zelda Remastered") pack declares, in its hires.txt:

    <patch>ZeldaHD.ips,DAB79C84934F9AA5DB4E7DAD390E5D0C12443FA2

i.e. it expects Mesen to apply ZeldaHD.ips to a clean PRG0 ROM (SHA-1 above)
before running. Our pipeline is a STATIC recompiler: the ROM's code is
translated to C at build time, so the patch cannot be applied at runtime.
The correct equivalent is to apply the IPS to a throwaway copy of the stock
ROM at regen time and recompile from THAT (the SNES MSU pattern — see
snesrecomp/.../tools/apply_msu_patch.py).

What the patch actually contains (decoded from the 87 records, 3232 bytes):
  - Audio plumbing: new 6502 subroutines injected into free (0xFF) ROM space
    in banks 0/4/5/6/7, with JSR/JMP hooks added to existing code. The pack
    author labels the <patch> block "sound patch" — these expose sound-IDs so
    the pack's <bgm>/<sfx> can replace NES audio.
  - Content: re-scripted in-game text (banks 1/2/6) + some palette/attribute
    data (bank 5), i.e. the "Remastered" presentation.
The HD *graphics* conditions are gated mostly on STOCK RAM ($EB RoomId /
$EC NextRoomId etc.), so backgrounds/tiles do not strictly require this patch;
building from the patched ROM simply matches the exact ROM the pack was
authored against (graphics + audio + text), eliminating calibration drift.

This is opt-in: stock builds use the unpatched ROM and are byte-identical to
today. Only the HD build feeds the patched ROM to the recompiler.

Usage:
    python tools/apply_hd_patch.py --rom zelda.nes \
        --ips <pack>/ZeldaHD.ips --out build/zelda_hd.nes
"""
import argparse
import hashlib
import os
import sys

# Clean PRG0 "Legend of Zelda, The (USA).nes" (iNES, 131088 bytes) — the SHA-1
# the pack's <patch> line targets.
VANILLA_PRG0_SHA1 = "dab79c84934f9aa5db4e7dad390e5d0c12443fa2"


def rom_sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def apply_ips(rom: bytearray, ips: bytes) -> int:
    if ips[:5] != b"PATCH":
        raise ValueError("not an IPS file (missing PATCH magic)")
    i, records = 5, 0
    while True:
        if ips[i:i + 3] == b"EOF":
            break
        off = (ips[i] << 16) | (ips[i + 1] << 8) | ips[i + 2]
        i += 3
        size = (ips[i] << 8) | ips[i + 1]
        i += 2
        if size == 0:  # RLE record
            run = (ips[i] << 8) | ips[i + 1]
            val = ips[i + 2]
            i += 3
            end = off + run
            if end > len(rom):
                rom.extend(b"\x00" * (end - len(rom)))
            for j in range(off, end):
                rom[j] = val
        else:
            end = off + size
            if end > len(rom):
                rom.extend(b"\x00" * (end - len(rom)))
            rom[off:end] = ips[i:i + size]
            i += size
        records += 1
    return records


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    here = os.path.dirname(os.path.abspath(__file__))
    ap.add_argument("--rom", required=True, help="stock Zelda PRG0 ROM (iNES)")
    ap.add_argument("--ips", default=os.path.join(here, "..", "hdpatch", "ZeldaHD.ips"),
                    help="ZeldaHD.ips (default: ../hdpatch/ZeldaHD.ips)")
    ap.add_argument("--out", default=os.path.join(here, "..", "build", "zelda_hd.nes"),
                    help="output patched ROM for regen (default: ../build/zelda_hd.nes)")
    ap.add_argument("--expect-sha1", default=VANILLA_PRG0_SHA1,
                    help="ROM hash the patch targets (for the mismatch warning)")
    args = ap.parse_args()

    rom = bytearray(open(args.rom, "rb").read())
    got = rom_sha1(bytes(rom))
    if args.expect_sha1 and got.lower() != args.expect_sha1.lower():
        sys.stderr.write(
            "\n*** WARNING: HD patch / ROM mismatch ***\n"
            f"  {args.rom}\n"
            f"    sha1 : {got}\n"
            f"    expect: {args.expect_sha1}  (clean PRG0, the patch's target)\n"
            "  ZeldaHD.ips is written for that exact ROM. Applying it to a\n"
            "  different image may produce a broken ROM and a non-working build.\n"
            "  Proceeding anyway.\n\n")
    else:
        print(f"[apply_hd_patch] ROM matches target PRG0 (sha1 {got})")

    ips = open(args.ips, "rb").read()
    n = apply_ips(rom, ips)
    open(args.out, "wb").write(rom)
    print(f"[apply_hd_patch] applied {n} IPS records -> {args.out} "
          f"({len(rom)} bytes, sha1 {rom_sha1(bytes(rom))})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
