/*
 * extras.c — The Legend of Zelda game-specific runner hooks
 *
 * Implements game_extras.h for Legend of Zelda.
 */
#include "game_extras.h"
#include "nes_runtime.h"
#include <stdio.h>
#include <string.h>

/* ---- game_extras.h implementation ---- */

uint32_t game_get_expected_crc32(void) { return 0xD7AE93DFu; }

const char *game_get_name(void) { return "The Legend of Zelda"; }

void game_on_init(void) {
}

void game_on_frame(uint64_t frame_count) {
    (void)frame_count;
}

void game_post_nmi(uint64_t frame_count) {
    (void)frame_count;
}

int game_handle_arg(const char *key, const char *val) {
    (void)key;
    (void)val;
    return 0;
}

const char *game_arg_usage(void) {
    return NULL;
}

/*
 * SRAM code dispatch override.
 *
 * During init, Zelda copies ROM bank 1 $A500 → SRAM $6C90 (~4720 bytes).
 * The game then calls subroutines in SRAM for save file management,
 * because the routines need to bank-switch the ROM window while running.
 *
 * In our static recompilation, bank switching is just a pointer change,
 * so we redirect SRAM calls to the original ROM source in bank 1.
 */
int game_dispatch_override(uint16_t addr) {
    if (addr >= 0x6C90 && addr < 0x8000) {
        int save_bank = g_current_bank;
        uint16_t rom_addr = 0xA500 + (addr - 0x6C90);
        g_current_bank = 1;
        call_by_address(rom_addr);
        g_current_bank = save_bank;
        return 1;
    }
    return 0;
}
