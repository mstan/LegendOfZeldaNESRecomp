# Startup and crash reports

v1.9.1 includes **Startup and Crash Diagnostics** in the launcher's **Mods**
page, under Developer. It is disabled by default.

1. Enable it before clicking **PLAY**.
2. Reproduce the problem once, then close the program if it is still running.
3. Open `diagnostics` beside `LegendOfZeldaNESRecomp.exe`. Share the newest
   `.log` and its matching `.dmp`, if one exists, with your bug report.
4. Include your Windows version, GPU, ROM revision, enabled mods, and whether
   you extracted into a fresh folder or updated an existing installation.
5. Disable the mod after testing.

Enabling the mod begins capture immediately, before PLAY verifies the ROM and
before the launcher releases its graphics resources. The saved selection also
enables capture before the launcher opens on later runs. Session files have UTC
timestamps and process IDs; a new run does not overwrite earlier evidence.

The log records the build, Windows and graphics adapter information, launcher
handoff, display/audio settings, renderer initialization, periodic progress,
errors and exit status. Windows unhandled exceptions and aborts also attempt a
small minidump containing the failing thread's exception context. Ordinary
startup errors produce a log and, for windowed launches, an error dialog;
they do not produce a crash dump. Forced termination and power loss cannot
produce a dump. The mod does not resolve the underlying crash.

If the program folder is read-only, Windows uses
`%LOCALAPPDATA%\NESRecomp\diagnostics`. Nothing is uploaded automatically.
Logs and dumps may contain local file paths and process memory; review them
before sharing. Keep them out of public posts if they contain personal details.
Inherited command-line output redirection is preserved.

For development, `NESRECOMP_DIAGNOSTICS_DIR` selects an existing writable
directory. Headless runs read mod selections only with `--mods-root DIR`.
The separately downloadable symbols archive contains the exact release PDB;
maintainers use it with the corresponding executable to inspect a dump.
