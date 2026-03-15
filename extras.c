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
