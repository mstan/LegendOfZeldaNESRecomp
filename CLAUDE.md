# Legend of Zelda NES Recomp

This is a static NES recompiler project for The Legend of Zelda.
See `nesrecomp/CLAUDE.md` for the full framework rules (RULE 0, RULE 1, RULE 2, build commands, etc.).

---

## Cutting a release — TWO builds (stock + HD)

This project ships **two executables**, because a static recompiler bakes one ROM's
code into each exe:

| Exe | Recompiled from | What it is |
|-----|-----------------|------------|
| `LegendOfZeldaNESRecomp.exe` (default) | stock `Zelda # NES.NES` | **pure stock** Zelda — no enhancements; launcher hides the HD-pack panel; runtime never loads a pack |
| `LegendOfZeldaNESRecomp-HD.exe` | patched `build/zelda_hd.nes` | **Zelda Remastered** — patched gameplay/text/audio; HD-pack panel + loading enabled; the HD texture pack is calibrated against this ROM |

**To build both: run `_zelda_release.bat`.** It builds the recompiler, derives the
patched ROM (`tools/apply_hd_patch.py` + `hdpatch/ZeldaHD.ips`), regens *both* ROMs
(renaming the recompiler's `generated/zelda_*` output to `zelda_stock_*` /
`zelda_hd_*`), then configures and builds both CMake targets into `build_release/`.
You must supply a legitimate stock PRG0 ROM (SHA-1
`dab79c84934f9aa5db4e7dad390e5d0c12443fa2`); no ROM is committed.

**How the split is wired** (all keyed off one compile flag, `NESRECOMP_GAME_NO_HDPACK`,
which the stock target defines in `CMakeLists.txt`):
- `extras.c` → `game_get_expected_crc32()` returns stock `0x3FE272FB` vs patched `0xFD9C577F`.
- `hdpack.c` → `hdpack_load_from_config()` early-returns (no pack) on the stock build.
- `launcher.c` → sets `hdpack_supported = 0`, so the launcher hides the whole HD-pack
  panel (mirrors the existing `widescreen_supported` gate).

Dev tip: build just one variant with `cmake --build build_release --target LegendOfZeldaNESRecomp`
(or `…-HD`). The "NOMAP 0.2 BY SNARF" title credit is part of the patched ROM (PRG
`0x1A8B7`), so it only appears on the HD build — that's intended.

---

## TCP Debug Server (CRITICAL - use this for debugging)

The game executable runs a **TCP debug server on localhost:4370** (enabled when `debug.ini` exists next to the exe). Connect with any TCP client (e.g., `ncat`, Python socket, or the Bash tool via `echo '...' | ncat localhost 4370`).

**Protocol:** JSON-over-newline. Send `{"cmd":"<name>", ...}\n`, receive JSON response.

### Built-in commands (debug_server.c)

| Command | Description |
|---------|-------------|
| `ping` | Heartbeat check |
| `frame` | Current frame number |
| `get_registers` | CPU registers (A, X, Y, S, P) |
| `read_ram` | Read RAM bytes: `{"cmd":"read_ram","addr":0x70,"len":12}` |
| `dump_ram` | Hex dump of RAM range |
| `write_ram` | Write RAM bytes |
| `read_ppu` | Read PPU memory |
| `mapper_state` | Current mapper/bank state |
| `watch` | Set watchpoint on RAM address |
| `unwatch` | Remove watchpoint |
| `set_input` | Override controller input |
| `clear_input` | Clear input override |
| `pause` | Pause execution |
| `continue` | Resume execution |
| `step` | Step one frame |
| `run_to_frame` | Run to a specific frame number |
| `history` | Query ring buffer (36000 frames / ~10 min) |
| `get_frame` | Get specific frame record from ring buffer |
| `frame_range` | Get range of frame records |
| `frame_timeseries` | Time series data from ring buffer |
| `first_failure` | Find first verify-mode failure in history |
| `ppu_state` | PPU register state |
| `watchdog_status` | Watchdog trigger info |
| `call_stack` | Recomp call stack (if RECOMP_STACK_TRACKING enabled) |
| `quit` | Exit the game |

### Zelda-specific commands (extras.c)

| Command | Description |
|---------|-------------|
| `zelda_state` | Game mode, room, Link position, HP, sword, sub-mode |
| `entity_snapshot` | All 12 entity slots (type, x, y, dir, state, meta, timer, uninit) |
| `entity_slot` | Detailed single slot: `{"cmd":"entity_slot","slot":1}` |
| `echo_cmd` | Echo test |

### Quick usage from Bash

```bash
# One-shot command
echo '{"cmd":"zelda_state"}' | ncat localhost 4370

# Read RAM (e.g., entity types at $034F-$035A)
echo '{"cmd":"read_ram","addr":847,"len":12}' | ncat localhost 4370

# Pause, inspect, continue
echo '{"cmd":"pause"}' | ncat localhost 4370
echo '{"cmd":"entity_snapshot"}' | ncat localhost 4370
echo '{"cmd":"continue"}' | ncat localhost 4370
```

### Ring buffer

36000-frame history (~10 min at 60fps). Each record includes:
- CPU state (A, X, Y, S, P)
- PPU state (PPUCTRL, PPUMASK, scroll X/Y)
- Mapper bank, controller buttons
- Game-specific: 16 bytes filled by `game_fill_frame_record()` in extras.c
- Last recomp function name
- Verify mode diffs (if enabled)

### Zelda game_data layout in frame records (extras.c)

| Byte | Address | Meaning |
|------|---------|---------|
| 0 | $12 | GameMode |
| 1 | $EB | CurrentRoom |
| 2-12 | $34F+i | ObjType[0..10] |
| 13 | $11 | GameSubmode |
| 14 | $0F | Uninit flag during UpdateObject |
| 15 | $0340 | CurObjIndex |

---

## Key RAM Addresses (Zelda)

| Address | Name |
|---------|------|
| $0012 | GameMode |
| $0011 | GameSubmode |
| $0013 | SubMode |
| $00EB | CurrentRoom |
| $0070+i | ObjX[i] |
| $0084+i | ObjY[i] |
| $0098+i | ObjDir[i] |
| $00AC+i | ObjState[i] |
| $034F+i | ObjType[i] |
| $0405+i | ObjMetastate[i] |
| $0028+i | ObjTimer[i] |
| $0492+i | ObjUninitialized[i] |
| $066F | Hearts/HP |
| $0657 | Sword |
| $0340 | CurObjIndex |
| $00E0 | Paused |
| $00E1 | MenuState |

---

## Debugging Workflow

1. Launch game (with `debug.ini` next to exe)
2. Connect via TCP: `ncat localhost 4370`
3. Use `zelda_state`, `entity_snapshot`, `read_ram` to inspect live state
4. Use `pause`/`step`/`continue` for frame-by-frame analysis
5. Use `watch` for RAM change notifications
6. Use `history`/`frame_range` to look back at what happened
