#pragma once

#include <stdint.h>

void zelda_voxel_configure_arg(const char *value);
void zelda_voxel_init(void);
void zelda_voxel_update_hotkey(void);
void zelda_voxel_post_render(uint32_t *framebuffer);
