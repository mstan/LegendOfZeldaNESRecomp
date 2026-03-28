# Legend of Zelda NES Recomp

This is a static NES recompiler project for The Legend of Zelda.
See `nesrecomp/CLAUDE.md` for the full framework rules (RULE 0, RULE 1, RULE 2, build commands, etc.).

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
