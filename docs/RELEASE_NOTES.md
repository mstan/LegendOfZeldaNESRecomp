# v1.10.0 — Seamless Overworld (experimental)

This minor feature release adds **Seamless Overworld** as the first bundled
entry in **Mods → Camera**, with a separate Experimental badge. Enable it before
PLAY; it is disabled by default.

- Follow Link continuously through ordinary overworld and connected dungeon
  boundaries, with neighboring terrain and live actors visible ahead.
- Fit the view to the window's aspect ratio, including portrait and ultrawide.
  Use the mouse wheel for pixel zoom and F4 for the whole map.
- Keep the expanded scene during inventory, bomb flashes, death and portal
  loading. Game Over and Continue use a responsive layout.
- Preserve native puzzle routing at caves, stairs, dungeon entrances and maze
  portals. Cave interiors extend their boundary terrain across the view.
- Include actual overworld, dungeon and whole-map pictures in the README.

The mod is experimental and local-only. Enemy migration and projectile
interactions across room boundaries are incomplete; the whole-map simulation
is slower. Later dungeons, bosses, both quests and strict replay timing remain
open validation areas. It is mutually exclusive with voxel and Remastered views.

Focused checks cover real overworld crossings, a dungeon key door, cave/Level 1
entry and exit, inventory, bombs, lethal enemy contact, Game Over/Continue/Retry,
window resizing and loading-state picture restoration. No full-game audit is
claimed. Existing native progress is preserved; save states are build-specific.

Supply your original USA PRG0 ROM. No ROM, third-party HD art, personal saves
or settings are included. See README.md and docs/SEAMLESS-WORLD.md for controls,
pictures and the remaining experimental limits.
