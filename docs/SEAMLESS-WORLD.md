# Seamless world development

Owner request, 2026-10-27; tracking: `beads-2dw.18.3`.
Worktree branch: `feat/seamless-world-20261027`, based on released v1.9.5.

The target is continuous player movement and camera tracking through both the
overworld and dungeons, with neighboring actors active before entering view.
Window size and zoom should control how much real world is visible, up to an
entire map. Native progress, combat, puzzles and progression must remain useful.
The owner explicitly selected **preserve puzzles and use portals where needed**
for Lost Woods, warps, caves and stairs. Do not flatten those routes into a
geographic shortcut.

## Current slice

`--world-view follow` enables a development terrain camera; `atlas` requests the
whole overworld; `off` retains the ordinary game. `--world-size WxH` fixes a
development viewport. Otherwise window resizing changes the world viewport and
the mouse wheel changes pixel zoom. This is intentionally absent from the Mods
catalog until gameplay is ready.

The original USA PRG0 ROM is checked by payload CRC32. Neighbor terrain comes
from calls to the original `SwitchBank` ($FFAC), `LayOutRoom` ($A8C4) and
`FillPlayAreaAttrs` ($83D7) in a fully isolated machine. Existing engine APIs
restore all guest memory, hardware and compiled RAM views. Secrets and quest
tables remain the game's own; no extracted Nintendo data is checked in. The
active room uses its live tile map and actual frame, preserving its actors and
dynamic terrain. The world camera clamps at geographic world edges, not room
edges. Caches refresh when the level, quest or room world flags change.

**This slice is not seamless gameplay.** Original transitions, room ownership,
and actor slot limits still apply. Adjacent actors and dungeons are not yet
implemented. The TCP `zelda_world` response exposes these gaps explicitly.
`zelda_world_probe` compares a decoded room with the live tile/attribute planes
and compares complete serialized machine state before/after the isolated call.

## Implementation decisions and remaining gates

1. Verify original decoder output and continuous camera against actual ROM
   gameplay. Validate arbitrary viewport shapes, atlas bounds and native HUD.
2. Introduce room instances with persistent local actors, projectiles, items,
   dynamic terrain and local timers. Keep inventory, HP, keys, progress flags,
   native save data and global RNG ownership explicit. Reuse original actor
   routines where their local-coordinate and slot contracts can be maintained.
3. Replace ordinary room handoffs with continuous signed world coordinates.
   Keep neighboring instances ticking during handoff. Original edge cutoffs,
   transition resets, spawn initialization and room history cannot simply be
   skipped: each has gameplay effects that need an explicit equivalent.
4. Preserve exceptional routes as portals. Derive dungeon room ownership from
   level topology, door state and stairs. Shared underworld attribute tables
   contain rooms belonging to other levels; rendering all rows would be wrong.
5. Separate visibility from activation. Prewarm at least the neighboring ring
   outside camera bounds, retain actors across nearby crossings, and budget
   whole-map views without pretending static enemy icons are live actors.
6. Validate representative overworld seams in all directions, dungeon doors,
   locked and shutter doors, combat across visible boundaries, secrets,
   death/retry, cave/stair portals, Lost Woods, both quests, native saves, and
   current-build state/rewind. Record passed/failed/unverified individually.
7. Integrate as an optional enhancement, preserve stock launches and existing
   presentation choices, then prepare a small owner playtest before any release.

## Reuse considered

- Pinned `aldonunez/zelda1-disassembly` at
  `50a1c869a8d8e2eb8b5b60acea325f44b4341762`: complete source basis for
  memory ownership, room decoding, screen edge checks and original behavior.
- Existing Minish Cap Zelda foreign-world work (`beads-p42`, `beads-r0h`):
  useful verified ROM spans and data semantics, but incomplete native gameplay;
  not a complete replacement for the current Zelda simulation.
- `aldonunez/Loz_enhanced` at
  `b6d6346c62b9032cfe3e3dd358c17a3892497ff1` (Apache-2.0): substantial native
  gameplay reference, but its singleton world, room-local actor slots, original
  scrolling, Allegro renderer and separate resource format still need major
  changes. Do not introduce a second application/runtime merely for its camera.

Ground-truth reference:
https://github.com/aldonunez/zelda1-disassembly

No feasibility claim about terrain rendering establishes correctness of the
remaining gameplay work. Whole-map terrain is small (4096 x 1408 pixels);
room simulation and game rules, rather than map storage, are the main risk.
