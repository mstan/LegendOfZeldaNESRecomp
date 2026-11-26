#include "cyc_mod.h"
#include "mod_runtime.h"
#include <stdlib.h>

static int native_life_meter(NESModHdNativeArea *area)
{
    enum { GAME_MODE = 0x12, GAMEPLAY = 5, PAUSED = 0xe0 };
    if (cyc_mod_peek(GAME_MODE) != GAMEPLAY || cyc_mod_peek(PAUSED))
        return 0;

    /* The original game's life meter occupies this part of the status area.
     * Sample the already rendered native pixels, including empty hearts. */
    *area = (NESModHdNativeArea){176, 40, 240, 64};
    return 1;
}

NES_MOD_CONSTRUCTOR(register_remastered_artwork)
{
    static const NESModHdArtworkProvider provider = {
        .id = "legend-of-zelda.remastered.artwork",
        .game_id = "legend-of-zelda-us",
        .rom_crc32 = "3fe272fb",
        .rules_sha1 = "e1547cb505407c9ede3000c84d99d06a58136945",
        .native_area = native_life_meter,
    };
    if (!nes_mod_register_hd_artwork_provider(&provider))
        abort();
}
