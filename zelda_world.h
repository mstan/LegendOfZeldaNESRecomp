#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

/* Experimental continuous room simulation and adaptive world presentation.
 * Optional, local play only; ordinary stock launches retain their behavior. */
void zelda_world_reset(void);
void zelda_world_state_loaded(void);
void zelda_world_frame_end(void);
bool zelda_world_option(const char *name, const char *value);
void zelda_world_event(const SDL_Event *event);
const uint32_t *zelda_world_present(int *width, int *height);
void zelda_world_tcp_setup(void);
