# v1.9.1 — startup and crash diagnostics

- Adds a bundled **Startup and Crash Diagnostics** mod, disabled by default.
  Enable it in Mods before PLAY to record startup, host settings, renderer,
  progress and exit details. Windows crashes attempt a matching minidump.
- Capture begins when the mod is enabled, including the first PLAY handoff.
  Persistent selections start capture before the launcher opens on later runs.
- Displays error dialogs for game SDL initialization, window/renderer creation
  and ROM-loading failures instead of silently returning to the desktop.
- Retains matching release symbols in a separate download for dump analysis.

Share the newest `.log` and matching `.dmp`, if present, from `diagnostics`
beside the executable. See DIAGNOSTICS.md for fallback storage and privacy.
The reported v1.9.0 PLAY crash remains under investigation; this release adds
evidence collection and does not claim to fix an unconfirmed cause.

The cycle backend, stock ROM requirement, existing voxel modes and HD Mods
workflow are retained. No ROM, third-party HD art, settings or personal progress
is included. Legacy binary save states still require the legacy backend.