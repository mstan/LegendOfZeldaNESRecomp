#include "mod_runtime.h"
#include "zelda_voxel.h"

#include <stdio.h>

static void reset_voxel_diorama(void) {
    zelda_voxel_set_mod_enabled(0);
}

static void activate_voxel_overworld(void) {
    const char *package = "legend-of-zelda.enhancement.voxel-diorama";
    const char *feature = "voxel-diorama";
    zelda_voxel_configure_mod(
        0,
        nes_mod_get_option_int(package, feature, "pitch", 35),
        nes_mod_get_option_int(package, feature, "yaw", -20),
        nes_mod_get_option_int(package, feature, "roll", 0),
        nes_mod_get_option_int(package, feature, "zoom", 100),
        nes_mod_get_option_int(package, feature, "sprite-scale", 135));
    zelda_voxel_set_mod_enabled(1);
}

static void activate_voxel_first_person(void) {
    const char *package =
        "legend-of-zelda.enhancement.voxel-first-person";
    const char *feature = "voxel-first-person";
    zelda_voxel_configure_mod(
        1,
        nes_mod_get_option_int(package, feature, "pitch", 0),
        nes_mod_get_option_int(package, feature, "yaw", 0),
        nes_mod_get_option_int(package, feature, "roll", 0),
        nes_mod_get_option_int(package, feature, "zoom", 100),
        nes_mod_get_option_int(package, feature, "sprite-scale", 115));
    zelda_voxel_set_mod_enabled(1);
}

NES_MOD_CONSTRUCTOR(zelda_register_voxel_diorama_plugin) {
    int reset_registered =
        nes_mod_register_reset_callback(reset_voxel_diorama);
    int overworld_registered = nes_mod_register_activation_plugin(
        "legend-of-zelda.voxel-diorama", activate_voxel_overworld);
    int first_person_registered = nes_mod_register_activation_plugin(
        "legend-of-zelda.voxel-first-person",
        activate_voxel_first_person);
    if (!reset_registered || !overworld_registered ||
        !first_person_registered)
        fprintf(stderr, "[Mods] Failed to register Zelda voxel plugin\n");
}
