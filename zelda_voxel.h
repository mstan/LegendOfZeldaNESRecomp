#pragma once

#include <stdint.h>
#include <SDL.h>

void zelda_voxel_set_mod_enabled(int enabled);
void zelda_voxel_configure_mod(int first_person,
                               int pitch, int yaw, int roll,
                               int zoom_percent, int sprite_scale_percent);
void zelda_voxel_handle_event(const SDL_Event *event);
void zelda_voxel_init(void);
void zelda_voxel_update_hotkey(void);
void zelda_voxel_post_render(uint32_t *framebuffer);
