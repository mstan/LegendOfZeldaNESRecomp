#include "cyc_mod.h"
#include "mod_function_hooks.h"
#include "mod_runtime.h"
#include <stdlib.h>

enum {
    PLAY_UPDATE = 0xec1b,
    FLUTE_TIMER = 0x003c,
    PAUSED = 0x00e0,
    MENU_STATE = 0x00e1,
    BUTTONS_PRESSED = 0x00f8,
    BRIGHTENING_ROOM = 0x051e,
    TUNE_REQUEST = 0x0602,
    ITEM_SELECTION = 0x0656,
    INVENTORY = 0x0657,
    BOW = 0x065a,
    LETTER = 0x0666,
    BOOMERANG = 0x0674,
    MAGIC_BOOMERANG = 0x0675,
    SELECT = 0x20,
    POTION_SLOT = 7,
    LETTER_SLOT = 15,
    INVENTORY_SLOTS = 9,
};

static const char hook_id[] = "zelda.remastered.quick-items";

static int selectable_item(int slot) {
    if (slot == 0)
        return cyc_mod_peek(BOOMERANG) || cyc_mod_peek(MAGIC_BOOMERANG) ? 0 : -1;
    if (slot == 3 || (slot == 2 && !cyc_mod_peek(BOW)))
        return -1;
    if (cyc_mod_peek(INVENTORY + slot))
        return slot;
    if (slot == POTION_SLOT && cyc_mod_peek(LETTER))
        return LETTER_SLOT;
    return -1;
}

static int quick_items(uint16_t pc) {
    (void)pc;
    uint8_t pressed = cyc_mod_peek(BUTTONS_PRESSED);
    if (!(pressed & SELECT) || cyc_mod_peek(MENU_STATE) || cyc_mod_peek(PAUSED) ||
        cyc_mod_peek(FLUTE_TIMER) || cyc_mod_peek(BRIGHTENING_ROOM))
        return 0;

    /* The original update still owns movement, B-item use and HUD drawing.
     * Consume the already-latched Select edge before its pause toggle. */
    cyc_mod_poke(BUTTONS_PRESSED, pressed & ~SELECT);
    int current = cyc_mod_peek(ITEM_SELECTION);
    int slot = current == LETTER_SLOT ? POTION_SLOT : current;
    if (slot >= INVENTORY_SLOTS)
        slot = INVENTORY_SLOTS - 1;
    for (int checked = 0; checked < INVENTORY_SLOTS; ++checked) {
        slot = (slot + 1) % INVENTORY_SLOTS;
        int item = selectable_item(slot);
        if (item < 0)
            continue;
        if (item != current) {
            cyc_mod_poke(ITEM_SELECTION, (uint8_t)item);
            cyc_mod_poke(TUNE_REQUEST, cyc_mod_peek(TUNE_REQUEST) | 1);
        }
        break;
    }
    return 0;
}

static void activate(void) {
    if (!nes_mod_set_function_hook_enabled(hook_id, 1))
        abort();
}

static void reset(void) {
    nes_mod_set_function_hook_enabled(hook_id, 0);
}

NES_MOD_CONSTRUCTOR(register_quick_items) {
    if (!nes_mod_register_function_entry_plugin(hook_id, PLAY_UPDATE, quick_items) ||
        !nes_mod_register_reset_callback(reset) ||
        !nes_mod_register_activation_plugin("legend-of-zelda.remastered.quick-items", activate))
        abort();
}
