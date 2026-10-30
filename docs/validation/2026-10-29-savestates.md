# Zelda save-state rejection after changing presentation

Tracked by `beads-2dw.18.6`; released in v1.10.1. Framework and recomp-ui pins
remain `23e87d71f6da42e22450323d882047b90363798d` and
`c11a4b6c649164b6951eff107f4607dcef117dd5`.

## Cause and change

The published v1.10.0 executable reproduces the reported
`mod state 'zelda.cycle.voxel' is incompatible; nothing loaded` error:
configure first-person voxel, disable voxel, save, then load in a fresh stock
or Seamless Overworld process. The disabled renderer retained its first-person
flag and camera defaults. Its unconditional save record compared those unused
values with fresh-process defaults and rejected the entire machine snapshot.

The voxel adapter now writes an empty payload while disabled. Empty/missing
records are accepted only while disabled. Existing nonempty disabled records
still undergo structural/data validation, but their dormant camera preset is
not treated as compatibility identity and is not restored. Active voxel mode
and preset checks remain, as does preflight rejection before machine mutation.
No shared serializer, CPU/backend, other mod, native save path or dependency
pin changes are included.

## Validation

Windows x64 MinGW Release/HLE/native builds; automated launches hidden with
dummy audio/video, `--no-save`, and disposable profiles. SERVER capacity was
reserved through `benchmark_hosts.py`; the local PC's serial builds also used
capacity reservations. Other owner work was left running.

- Published v1.10.0: both history-switch cases save successfully and fail on
  restart with exit 2 and the exact reported error. Cold, unchanged profiles
  already roundtrip; this is not a blanket failure of voxel serialization.
- Candidate: stock, diorama, first person, Seamless Overworld, and both inactive
  history cases pass fresh-process load and 24-frame gameplay replay after the
  recorded 2,400-frame route. Every machine/mod chunk except the execution-path
  `TRAC` record, plus the final presented PNG, matches the continuous run.
- Both previously rejected v1.10.0 history-switch states load successfully.
- Active/inactive and diorama/first-person mismatches remain rejected, as do
  invalid versions, truncated payloads, missing active records and NaN heights.
- The actual F7 browser saves and loads, and F8 rewind resumes, in all four
  views through the game's TCP key interface. Logs confirm `State saved`,
  `State loaded` and `Resumed from rewind`.
- The legacy voxel translation unit also passes a C11 syntax check.

Reproduce the executable checks with `tests/savestate_probe.py --exe <exe>
--rom <stock-USA-PRG0-ROM> --out <disposable-directory> --input <route.txt>
--frame 2400`. The route is `ROUTE` in `tools/cycle_probe.py`. Without an input
route the probe defaults to a title-screen checkpoint.

Raw logs, state/picture samples, host receipts, source/binary identities and
release archive verification are retained under
`cycle-evidence/save-state-v1.10.1/`. No full-game or cross-version save-state
compatibility claim is made; native in-game progress is a separate contract.
