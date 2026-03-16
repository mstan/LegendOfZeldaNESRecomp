# Known Issues

## Gameplay

### ~~Black overworld background (no map tiles)~~ FIXED
**Root cause:** SRAM-sourced functions bank-switch at runtime, but the code generator hardcoded JSR/JMP calls to bank 1 (the source bank) instead of dispatching dynamically via `call_by_address()`. Room decompression code switched to bank 5 but then called bank 1 versions of utility functions, so column data never reached the PPU buffer.

**Fix:** Modified `code_generator.c` to detect SRAM-sourced functions via `is_sram_sourced()` and emit `call_by_address()` for switchable-range targets. Added `extra_func` entries for `$BFAC`/`$BF98` in banks 0-6 to `game.cfg`.

### Link sprite is only a head facing up
When gameplay starts, Link's sprite appears as just his head facing upward. No full body or animation.

## File Select Screen

### Save file slots show numbers instead of blank names
On the file select screen before any files are registered, all three save file slots display numbers where the player's name should be, despite having no saved files.

### Garbled cursor on file select
The cursor sprite on the file select screen is garbled/corrupted around the letter tiles. The SELECT option itself looks fine.

## HUD

### Garbled "DCE" in item counters
The rupee/key/bomb counters in the HUD display garbled text ("DCE") instead of proper values. Likely a tile mapping issue for the counter digits.
