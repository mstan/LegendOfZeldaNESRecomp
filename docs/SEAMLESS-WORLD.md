# Seamless world preview

Owner request: 2026-10-27. Bead: `beads-2dw.18.3`.
Isolated branch: `feat/seamless-world-20261027`, based on Zelda v1.9.5.
Engine preview pin: `159b9cf01714e593087f83215015ff9025b8c1a2`, based on
`721c68c313e729710203f6825e9bcb9042eb5bcc`. It adds a read-only drawable-size
API so Fit updates while menus own input; tracking: `beads-2dw.1.107`.

## Playable behavior

Enable **Seamless World (Experimental)** in Mods. It is an optional, local-play
presentation/gameplay enhancement, exclusive with the voxel display modes.
**Fit to screen is adaptive:** the window's width and height determine how much
actual world is visible at the selected pixel zoom. Wide and portrait windows
both expand the terrain/actors, with the native HUD centered. Mouse wheel changes
zoom; F4 toggles the entire current area's map. No extracted graphics are shipped.

Ordinary overworld crossings and ordinary open/key dungeon doors keep Link moving
in mode 5. They do not run the old mode 6/7 stop-and-scroll sequence. The camera
follows world coordinates; a native room handoff adopts the destination's already
running actors instead of reinitializing them. Neighboring rooms run before Link
enters. Native actor routines, collision, key consumption and dynamic terrain are
retained. Current save states include the room instances and local timers.

Caves, stairs, dungeon entrance/exit, Lost Woods and mountain-maze routing retain
native portals. Bombable and false-wall passages also currently retain their
original transitions. The owner selected preserving puzzle routing with portals.
Inventory slides over the retained expanded world. HUD backdrops and grayscale
bomb flashes span the entire viewport. Death retains that world camera, uses
the native death palette/Link animation, and fades the whole visible area.
Portal loaders retain the actual expanded source scene until the destination
terrain is ready. Cave, shortcut and cellar interiors extend their solid boundary
tiles across the extra view; native dialog, NPCs and items remain intact. Game
Over and Continue use the native font in a responsive layout, including a
window-width selection row. Native title/profile/ending artwork stays usable
on the adaptive canvas. Original portal timing and puzzle routing remain.

## Implementation

`zelda_world.c` owns the adaptive camera and cached room rasterization.
`zelda_world_rooms.c` owns persistent room state, activation, native-routine ticks,
collision lookahead and seam handoffs. Content-qualified hooks target the original
USA PRG0 routines. The ROM's payload CRC32 must be `3fe272fb`.

Each neighbor runs the original UpdateMode5Play in an isolated machine. Its local
actors, timers, RNG, tiles and palettes are retained. The real Link, CPU stack,
hardware, inventory, HP, audio and persistent saves keep their owner. Local actors
are activated in the visible area plus a halo; full-map view activates all owned
rooms. Dungeon membership follows reciprocal non-wall doors from the level entry,
excluding cellar layouts and unrelated rooms in the shared data tables.

Native routines decode terrain and door flags; temporary incoming shutter masks
and unlocked doors update cached door tiles without replacing pushed terrain.
Link's sword swing follows him across a handoff. Kill totals are committed once
when leaving a persistent room. Distant room rendering uses cached decoded tiles,
not a fresh per-pixel ROM interpretation each frame.

## Validation and burndown

Evidence is retained outside delivery folders in the task workspace
`G:/Temp/Codex/zelda-seamless-20261027`. Results are individual gates, not a claim
that a full playthrough or every dungeon has passed.

| Opportunity / gate | Status | Evidence or remaining work |
| --- | --- | --- |
| Decode real neighboring terrain | Passed | Native decoder; 128 overworld rooms; active tile/attribute equality; serialized machine isolation |
| Adjacent native actors before entry | Passed | Positions, native states and timers evolve while Link remains in his room |
| Continuous overworld crossings | Passed | East/west/north/south real passable seams, all mode 5, no reset of neighbor actors |
| Dungeon ownership | Representative pass | Level 1 membership, native entrance and surrounding actors; other levels unverified |
| Key door and open corridors | Representative pass | Real supplied key consumed; continuous northward crossing and return; open corridor both ways |
| Dungeon entrance/exit portals | Passed | Walked through the actual overworld entrance and back to its original source room |
| Portal scene presentation | Representative pass, owner recheck pending | Expanded source retained during real cave and Level 1 entry/exit; visible pixels checked outside the old native scene rectangle. Cave interior also tested wide/tall. Shortcut caves and cellars implemented, not yet exercised |
| Adaptive Fit to screen | Passed | Paused and running actual SDL resizing. Actual resizes: 1536x720 -> 512x240, 600x1200 -> 200x400, 3840x720 -> 1280x240 |
| Full-map presentation/activation | Functional, performance open | 4096x1472; 128 rooms / 579 actors live; approximately 19.4 ms median simulation plus 5.2 ms rendering on this host |
| Ordinary handoff cost | Representative pass | Final probe: maximum handoff simulation 6.646 ms; simulation p50/p95 2.350/6.354 ms; renderer p50/p95 0.317/0.573 ms. Component timings, not total host frame time |
| Portal/first activation costs | Known hitch | Full-area activation up to 137 ms in the probe; full-map first activation 135 ms. Ordinary crossings prewarm neighbors |
| Current-build save/load | Functional state pass; exact replay open | 180 replay frames end with identical machine/actor records and world picture. One first-resumed-frame native timing/hash discrepancy remains; rolling trace differs. Captured PCM export differs on restore in both stock and seamless runs. Do not call strict deterministic replay passed |
| Native saves | Preserved, coverage open | All QA disables native saves and uses disposable profiles; full normal-save gameplay audit remains |
| Shutters and block puzzles | Implemented, broad validation open | Native rules and held incoming door masks; remaining puzzle combinations need coverage |
| Combat and cross-boundary interactions | Partial | Native combat in active room; sword follows Link. Cross-room enemy migration and projectile interactions are incomplete |
| Death/Continue, inventory and bomb flashes | Representative pass, owner recheck pending | Real bombs and lethal contact in overworld/Level 1; actual Game Over text and Continue choices checked by pixels, ultrawide/portrait resizing, Select, Continue and Retry. Inventory restores the same camera |
| Secrets, bosses and other retry routes | Unverified | Need representative checks before release |
| Lost Woods, mountain maze and all warp routes | Routing preserved, unverified | Original portal path retained; puzzle sequences still need explicit tests |
| Second quest / every dungeon | Unverified | No full-content audit claimed |
| Human presentation/play check | Ready, awaiting observations | Prepared overworld, Level 1 key door and corridor states; lightweight selector saves Pass/Fail/Deferred notes |
| Portability / online play | Open / disabled | C11 and SDL implementation; Windows x64 built. Other targets unverified; mod explicitly local-only |

`probe-final-gameplay/results.json` contains the traversal/door/portal receipt;
`probe-fit-live/results.json` contains adaptive resizing and full-map results.
`replay/results.json` and `replay-stock/results.json` preserve the exact-replay
limitations. Source and binary identities are recorded in the handoff manifest.
Portal costs are separate from ordinary room crossings; the full-map performance
figures are not normal-view costs.

## Prepared human check

`tools/world_playtest.py` launches one game directly, in a normal window with
normal audio, an isolated profile and native saves disabled. Its localhost page
switches between the prepared setups and saves individual observations to
`observations.json`. F7 slots 1-3 mirror the setups; use later slots for manual
save/load. The helper exits when the game closes. It never touches owner profiles.

Developer CLI: `--world-view off|follow|atlas|seamless`; `--world-size WxH` fixes a
viewport for reproducible tests (omit it for adaptive Fit). TCP: `zelda_world`,
`zelda_world_room`, `zelda_world_timing`, and QA placement/edge/entrance commands.
Hidden automated probes mute output. The owner handoff enables normal audio.

## Reuse and references

The local disassembly is pinned to `aldonunez/zelda1-disassembly`
`50a1c869a8d8e2eb8b5b60acea325f44b4341762` (Variables.inc, Z_01/05/07.asm).
https://github.com/aldonunez/zelda1-disassembly

Substantial reuse was evaluated before implementing more room machinery:
`aldonunez/Loz_enhanced` at `b6d6346c62b9032cfe3e3dd358c17a3892497ff1`
(Apache-2.0) still has a singleton world, local actor slots, original scrolling,
Allegro and another resource format. Minish Cap's existing foreign Zelda world
provides useful verified data spans but is not a complete gameplay replacement.
Emerald and Minish Cap's adaptive-view plugins were consulted for Fit behavior:
real expanded content, window aspect in landscape/portrait, and anchored UI.

## Owner regression follow-up

The owner reported narrow bomb flashes, inventory falling back to a different
native room view, and narrow death presentation (supplied screenshot). All
three are tracked as owner findings, pending human recheck.

`probe-effects-final-overworld/results.json` and
`probe-effects-final-dungeon/results.json` exercise real input to open/close
inventory and place a bomb. Native lethal enemy contact is induced by placing
Link beside an existing enemy with low health, without setting a death mode or
completion flag. The death sequence, Continue page and return to gameplay retain
the viewport. Camera coordinates before/after inventory match. Background tests
use disposable profiles, muted output and disabled physical player inputs.
The owner checklist records flash, inventory, death, portal presentation and
Game Over observations separately.

Raster color updates now build only rooms intersecting the camera, so an
expanded bomb flash does not rebuild distant invisible terrain. Full-map views
still include every owned room; room simulation/activation rules are unchanged.

The owner subsequently identified boxed cave/dungeon transitions and Game Over
content inside the correctly sized output. Earlier size-only checks missed this;
they do not establish expanded scene presentation. `probe-portals-state/results.json`
now records real cave entry/exit, Level 1 entry/exit, cave resizing, and exact
picture/camera restoration from states saved during all four portal loaders.
`probe-portals-ui-final/results.json` and
`probe-portals-ui-dungeon/results.json` cover the actual Game Over text, native
Continue selection and window-spanning UI pixels. `probe-portals-seams-final`
rechecks ordinary crossings and key consumption. Original failures remain
recorded in `owner-handoff/reported-findings.json`, pending human recheck.

The retained portal source includes CHR, OAM, palette and terrain from before
the ROM replaces its data tables. It is serialized only during transitional
modes; normal gameplay states rebuild derived presentation data. This focused
portal picture check does not clear the separate strict replay timing gate.
