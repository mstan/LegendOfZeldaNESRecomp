#pragma once
#include <stdbool.h>
#include <stdint.h>

/* Raster inputs are the world's existing 16x8 grid of 256x176 room images.
 * revision changes whenever a room image is rebuilt. valid has128 owned/valid
 * flags. The caller owns the output; HUD and actors are drawn after begin. */
typedef struct {
    int width,height,camera_x,camera_y;
    uint32_t background;
    const uint32_t *rasters;
    const uint8_t *valid;
    uint64_t revision;
} ZeldaCanvasView;

void zelda_canvas_begin(uint32_t *canvas,const ZeldaCanvasView *view,bool retain);
void zelda_canvas_dirty(int x,int y,int width,int height);
void zelda_canvas_invalidate(void);
void zelda_canvas_release(void);
void zelda_canvas_stats(uint64_t *builds,uint64_t *reuses,uint64_t *bytes);
