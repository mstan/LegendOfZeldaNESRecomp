# LegendOfZeldaNESRecomp

Static recompilation of The Legend of Zelda (NES) for native PC.
Built with the [NESRecomp](https://github.com/mstan/nesrecomp) framework.

> **Status: Work in progress.** The game boots, title screen and file select are functional, overworld renders and scrolls between screens, and enemies spawn. Many features are incomplete or broken — see Known Issues below.

## What Works

- Title screen, demo playback, and file select menu
- Character creation / name entry (register mode)
- Overworld rendering with room-to-room scrolling
- Enemy spawning with correct metastate lifecycle (spawn clouds, movement)
- HUD partially renders (life meter, minimap outline)
- Basic Link movement and screen transitions

## Known Issues

- **Cave / dungeon entrance soft-locks** — entering a cave tile freezes the game. The entity spawning system for cave NPCs (old man, merchants) is incomplete.
- **Screen transition artifacts** — new areas briefly render as all-zero tiles before the correct map data loads. PPU nametable update timing issue.
- **Link sprite half-missing** — only the upper body renders on spawn. 8x16 sprite mode handling is incomplete.
- **HUD counters garbled** — rupee/key/bomb counters display "DCE" instead of proper digits. Tile mapping issue.
- **Fairy cursor garbled** — the file select fairy sprite is partially corrupted.
- **No save persistence** — SRAM battery-backed saves are not implemented yet.

## Quick Start

1. Download `LegendOfZeldaNESRecomp-windows-x64.zip` from [Releases](../../releases)
2. Extract and run `LegendOfZeldaNESRecomp.exe`
3. Select your Legend of Zelda (USA) ROM when prompted — the path is saved for future launches

## Controls

| NES Button | Keyboard |
|------------|----------|
| D-Pad      | Arrow keys |
| A          | Z |
| B          | X |
| Start      | Enter |
| Select     | Tab |

## Building from Source

Requires Visual Studio 2022 and CMake 3.20+.

```bash
git clone --recursive https://github.com/mstan/LegendOfZeldaNESRecomp
cd LegendOfZeldaNESRecomp
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Place your `Legend of Zelda (USA).nes` ROM in the build directory or select it at runtime.

## Architecture

This is a **static recompiler**, not an emulator. The original 6502 machine code is translated to C at build time, then compiled to native x64. The NES PPU, APU, and mapper are simulated by the runner library.

- `game.cfg` — recompiler configuration (bank switch routine, inline dispatch tables, extra function seeds)
- `extras.c` — game-specific hooks (SRAM dispatch override, debug server commands, entity diagnostics)
- `generated/` — auto-generated C code (do not edit manually)
- `nesrecomp/` — framework submodule (recompiler + runner)
- `tools/` — analysis and debug utilities

## Debug Infrastructure

The game includes a TCP debug server (port 4370) with a 36000-frame ring buffer, enabled by placing a `debug.ini` file next to the executable. See `tools/dbg.py` for the query interface.
