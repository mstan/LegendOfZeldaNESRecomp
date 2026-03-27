# Known Issues

## Gameplay

### ~~Black overworld background (no map tiles)~~ FIXED
**Root cause:** SRAM-sourced functions bank-switch at runtime, but the code generator hardcoded JSR/JMP calls to bank 1 (the source bank) instead of dispatching dynamically via `call_by_address()`. Room decompression code switched to bank 5 but then called bank 1 versions of utility functions, so column data never reached the PPU buffer.

**Fix:** Modified `code_generator.c` to detect SRAM-sourced functions via `is_sram_sourced()` and emit `call_by_address()` for switchable-range targets. Added `extra_func` entries for `$BFAC`/`$BF98` in banks 0-6 to `game.cfg`.

### Soft lock when entering a cave — likely entity spawning (CRITICAL, HIGHEST PRIORITY)
When Link enters a cave entrance, the game soft locks. The screen transition never completes and the game becomes unresponsive.

**What works:** Overworld room-to-room scrolling works correctly. The game mode transitions (modes $04→$05→$06→$07) complete normally, including the characteristic Zelda pause between screens. Walking between overworld screens and back is fine.

**What breaks:** Entering a cave tile triggers cave mode ($0D in RAM $0012), which dispatches via `func_EB30()` case 13 → `call_by_address(0xA6AD)` → `func_A6AD_b2()` (bank 2). This function uses an inline dispatch on sub-mode $0013 with cases 0, 1, 2 for the cave transition sequence.

**Likely cause — entity spawning:** Caves contain spawning entities (old man giving sword, merchants, NPCs). The entity spawning system is independently broken (see "Enemies don't spawn" below). If cave entity spawning uses the same broken code path, it could prevent the cave transition from completing — the game may wait for entities to finish spawning before advancing the cave sub-mode.

**Investigation notes:**
- Zero dispatch misses during gameplay — SRAM dispatch override in `game_dispatch_override()` works correctly for addresses $6C90–$7FFF
- The SRAM→ROM remap (e.g., SRAM $6DF1 → ROM $A661 bank 1) is confirmed working in the dispatch table
- Runtime architecture: NMI fires via `maybe_trigger_vblank()` on every `nes_read()`/`nes_write()`. During NMI, `s_vblank_firing` prevents re-entrant NMI. A true infinite loop inside NMI would freeze the game since no new frames would fire.
- Need a recording that actually walks into a cave to capture the exact failure point

**Key functions:**
- `func_EB30()` — main game mode dispatcher (fixed bank, dispatches on RAM $0012)
- `func_A6AD_b2()` — cave entrance handler (bank 2, dispatches on RAM $0013)
- `func_A6B8_b2()` — cave sub-mode 0: setup, calls SRAM function $6DF1
- `func_A734_b2()` — cave sub-mode 1: positioning/scroll
- `func_A803_b2()` — cave sub-mode 2: finalizes, resets mode to $00

### Enemies don't spawn (only smoke clouds)
Enemies show their initial spawn smoke/puff animation but never complete spawning into actual enemies. This applies to ALL enemy types (tektites, octoroks, etc.).

**Likely connection to cave soft lock:** Zelda's entity system handles both overworld enemies and cave NPCs/merchants. If the entity spawn completion logic is broken (preventing smoke→enemy transition), the same issue could prevent cave NPCs from spawning, which may be what blocks the cave transition from completing.

**Investigation needed:** Trace the entity spawn system in bank 4 (enemy AI bank). The spawn animation plays (smoke clouds render), so the initial spawn trigger works. The bug is in the transition from spawn animation → actual entity. Look for:
- Dispatch misses in bank 4 entity update functions
- Missing function entries for entity state machine transitions
- Bank switching issues during entity AI updates (bank 4 enemy logic may call SRAM or cross-bank functions)

### Link only has upper body half when spawning
When gameplay starts, Link's sprite is missing the lower half — only the upper body renders. Previously was just a head facing up; the SRAM scan improvement partially fixed this but the lower body tiles are still missing.

**Notes:** This is a sprite/OAM issue. Link uses 8x16 sprite mode. The upper body tiles render correctly but lower body tile indices may be wrong or CHR RAM may not have the correct tile data at those indices.

### New area renders as all-zeros briefly
When scrolling to a new overworld area, the new screen content initially appears as all zeros (blank tiles) before rendering properly. On real hardware / accurate emulators, the new column/row data is loaded incrementally during the scroll. Likely a PPU nametable update timing issue — the data eventually arrives but isn't ready when first displayed.

## File Select Screen

### Fairy cursor still somewhat garbled
The fairy cursor sprite on the file select screen is still somewhat garbled/corrupted, though the SELECT option text looks fine.

### ~~Save file slots show numbers instead of blank names~~ (needs re-test)
On the file select screen before any files are registered, all three save file slots display numbers where the player's name should be, despite having no saved files.

## HUD

### Garbled "DCE" in item counters
The rupee/key/bomb counters in the HUD display garbled text ("DCE") instead of proper values. Likely a tile mapping issue for the counter digits.

---

## Function Discovery (Recompiler)

### Register propagation enhancements (branch: `feature/register-propagation-dispatch`)

Improved static analysis in `function_finder.c` to auto-discover function entry points that previously required manual `extra_func` config entries. **14/16 extra_func entries now auto-discovered** (up from 12/16), with 100 new SRAM-region functions generated that were previously missing.

**Changes:**
- **Auto-derive switchable bank_switch:** Recognizes $BFAC as equivalent to $FFAC (fixed-bank routine mirrored at addr-$4000 in switchable banks). Discovers $BFAC/$BF98 across banks 0-6 automatically.
- **Deferred bank switch:** Applies effective_bank change AFTER JSR target is registered, preventing bank-switch routines from being added under the wrong bank context.
- **Backward table scan:** Scans backward from `JMP (indirect)` for split lo/hi table setup patterns (`LDA abs,X; STA zp; LDA abs,X; STA zp+1`). Found 0 targets for Zelda (handled by `inline_dispatch` config) but ready for future games.
- **SRAM region linear scan:** Discovers functions in SRAM-mapped code regions by seeding addresses after RTS/RTI boundaries. Found 41 candidates for Zelda, leading to 100 new generated functions (including callees discovered via BFS).

**Stats:** 4721 total functions (up from 4679), 2688 bank switches detected via register propagation.

### Remaining manual `extra_func` entries (2/16)

These two entries cannot be discovered by static analysis and will remain manual:

- **`$A69D` bank 1** — Mid-instruction jump target. Address $A69C contains `LDA $0342` (bytes `AD 42 03`), and code jumps to $A69D which re-interprets the operand bytes as a different instruction stream. This overlapping-code technique has no static reference to $A69D as a function entry.
- **`$E6B8` fixed bank** — Hidden behind inline dispatch table over-read. The inline dispatch scanner at $E5E2 reads a table of 2-byte addresses after the JSR; the table ends when a value < $8000 is encountered. At one call site ($E6A3), the table scanner over-reads past the real entries into code bytes, which happen to form plausible-looking addresses. The real function at $E6B8 is only reachable as a table entry but gets masked by the over-read. `pre_jsr_a` (the A register value before the JSR) is always unknown at this call site due to intervening JSR calls tainting the register.

**Future path to parity:** Emulator trace seeding (run game in emulator, log all PC values, cross-reference against discovered functions) or game-specific heuristics could eventually eliminate these last 2 manual entries.
