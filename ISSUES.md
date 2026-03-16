# Known Issues

## Gameplay

### Black overworld background (no map tiles)
When starting a new game, the overworld terrain/map tiles are completely absent. The nametable game area (rows 8-29) contains only blank tiles ($24). CHR RAM is populated and attributes are correct, so the issue is in the room data decompression path. The room loading state machine completes normally ($12: 3->4->5) and the room number is correct ($EB = 0x77), but tile data never reaches the nametable.

The room loading code runs from SRAM via dispatch override (extras.c). Decompression functions in bank 1 ($A6A6, $A661, $B0F5, $B1F8, etc.) are being called, but the decompressed data isn't making it to PPU nametable writes.

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
