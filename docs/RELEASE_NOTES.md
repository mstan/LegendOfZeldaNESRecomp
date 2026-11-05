# v1.12.0 — Stopwatch freeze scopes

Seamless Overworld now has a **Stopwatch scope** dropdown under its Gameplay
options: **Entire map** (the existing default), **Current viewport**, and
**Current room**. Viewport captures the visible world rectangle at pickup,
including adaptive Fit and ultrawide margins. Room captures the NES room Link
occupies at pickup. Moving, zooming or resizing afterward keeps the captured
area fixed; another stopwatch captures a new area. Current-build save states
retain the pickup area and reject invalid area data before loading.

Fresh profiles enable Seamless Overworld for adaptive widescreen by default.
Existing explicit display and mod choices are preserved. This experimental
mode keeps its documented local-play and cross-room interaction limits.
Native in-game saves and settings retain their existing behavior.

# v1.11.0 — HD rendering resolution

Zelda Remastered HD can now use resident GPU rendering with a selectable
resolution under **Graphics → HD resolution**. Choose the pack's authored
resolution or 1×, 2×, 4×, 8× and 16×. The 32× and 64× choices are experimental.
Rendering samples the original pack art; it does not generate additional detail.

The qualified RTX 3080 Ti gameplay interval ran at 93.24–108.30 FPS at true
16× (4096×3840), with no GPU fallback. This qualification applies to Remastered
HD; stock and voxel presentation retain their existing renderers. When the GPU
backend is unavailable, portable CPU rendering uses the pack's authored
resolution. Native gameplay and in-game saves retain their existing behavior.

# v1.10.1 — Save-state loading fix

Fix save states refusing to load in stock or Seamless Overworld after a
first-person voxel session. Disabled voxel settings were retained in the save
and incorrectly compared with a fresh session's camera defaults.

Disabled voxel views now contribute no camera payload. Existing disabled
records are validated and their unused camera settings are discarded. Active
voxel states still require the same voxel mode and configured camera preset;
invalid records are rejected before loading any machine state. Other mods and
native in-game saves retain their existing behavior.

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
