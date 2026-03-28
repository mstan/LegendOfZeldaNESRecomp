# Zelda NES Recomp — Live Debug Cheats

Requires `debug.ini` next to the exe. Connect via TCP to `localhost:4370`.

## Quick Usage

```bash
# One-shot command
echo '{"cmd":"write_ram","addr":"0x066D","val":"0xFF"}' | ncat localhost 4370
```

**Note:** The `val` parameter is parsed as hex when prefixed with `0x`. Always use `"val":"0xNN"` format.

## Max Inventory Script

Run `python3 C:/temp/zelda_max_save.py` while in-game, or send these individually:

| Address | Value | Effect |
|---------|-------|--------|
| `0x0657` | `0x03` | Magical Sword |
| `0x0658` | `0x10` | 16 Bombs |
| `0x0659` | `0x02` | Silver Arrows |
| `0x065A` | `0x01` | Bow |
| `0x065B` | `0x02` | Red Candle |
| `0x065C` | `0x01` | Recorder/Whistle |
| `0x065D` | `0x01` | Food/Bait |
| `0x065E` | `0x02` | Red Potion |
| `0x065F` | `0x01` | Wand/Rod |
| `0x0660` | `0x01` | Raft |
| `0x0661` | `0x01` | Book of Magic |
| `0x0662` | `0x02` | Red Ring |
| `0x0663` | `0x01` | Ladder |
| `0x0664` | `0x01` | Magic Key |
| `0x0665` | `0x01` | Power Bracelet |
| `0x0666` | `0x01` | Letter |
| `0x066A` | `0xFF` | 255 Rupees (display address) |
| `0x066B` | `0x08` | 8 Keys |
| `0x066C` | `0x10` | 16 Heart Containers |
| `0x066D` | `0xFF` | Rupee count (actual) |
| `0x066E` | `0xFF` | All 8 Triforce Pieces |
| `0x066F` | `0xFE` | Full Hearts |
| `0x0670` | `0x02` | Magical Boomerang |
| `0x0671` | `0x01` | Magic Shield |
| `0x067C` | `0x10` | Max bomb capacity |

## Useful Debug Commands

```bash
# Pause/resume
echo '{"cmd":"pause"}' | ncat localhost 4370
echo '{"cmd":"continue"}' | ncat localhost 4370

# Check game state
echo '{"cmd":"read_ram","addr":"0x12","len":2}' | ncat localhost 4370   # GameMode/SubMode
echo '{"cmd":"read_ram","addr":"0xEB","len":1}' | ncat localhost 4370   # Current Room

# Teleport (write room number)
echo '{"cmd":"write_ram","addr":"0xEB","val":"0x37"}' | ncat localhost 4370  # Room $37

# Entity snapshot
echo '{"cmd":"entity_snapshot"}' | ncat localhost 4370

# Frame-by-frame
echo '{"cmd":"step"}' | ncat localhost 4370
```

## Persistence

Die in-game and choose **SAVE** on the continue screen to persist inventory to `zelda.srm`.
Picking up items does NOT auto-save — only the death/save screen writes SRAM.
