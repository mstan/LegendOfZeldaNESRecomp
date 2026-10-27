#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

/* Development world-view foundation. Gameplay still uses the original room
 * simulation until the room ownership work is complete. Never enabled by
 * default or advertised as a finished seamless gameplay mode. */
void zelda_world_reset(void);
bool zelda_world_option(const char *name, const char *value);
void zelda_world_event(const SDL_Event *event);
const uint32_t *zelda_world_present(int *width, int *height);
void zelda_world_tcp_setup(void);
