/*
 * zelda_voxel.c -- Legend of Zelda profile for NESRecomp's voxel compositor
 *
 * The renderer remains presentation-only.  This profile derives geometry from
 * Zelda's live 32x22 PlayAreaTiles array and the same walkability boundary used
 * by the game.  No ROM assets are distributed or replaced.
 */
#include "zelda_voxel.h"

#include "nes_runtime.h"
#include "voxel_renderer.h"

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZELDA_PLAYFIELD_X       0
#define ZELDA_PLAYFIELD_Y       64
#define ZELDA_PLAYFIELD_WIDTH   256
#define ZELDA_PLAYFIELD_HEIGHT  176
#define ZELDA_TILE_COLUMNS      32
#define ZELDA_TILE_ROWS         22
#define ZELDA_TILE_COUNT        (ZELDA_TILE_COLUMNS * ZELDA_TILE_ROWS)
#define ZELDA_WIDE_MARGIN       85

/* CPU $6530 maps to g_sram[$0530].  Columns are contiguous 22-byte runs. */
#define ZELDA_PLAY_AREA_TILES   (g_sram + 0x0530)

static const int s_angles[] = { 0, 15, 35, 50, 75 };
static float s_heights[ZELDA_TILE_COUNT];
static uint8_t s_classification[ZELDA_TILE_COUNT];
static int s_queue[ZELDA_TILE_COUNT];
static int s_angle_index;
static int s_requested_at_startup;
static int s_key_was_down;

static int gameplay_scene_visible(void);

static int tile_index(int x, int y) {
    return x * ZELDA_TILE_ROWS + y;
}

static int overworld_walkable_exception(uint8_t tile) {
    static const uint8_t walkable[] = {
        0x8D, 0x91, 0x9C, 0xAC, 0xAD, 0xCC, 0xD2, 0xD5, 0xDF
    };
    for (unsigned i = 0; i < sizeof(walkable); i++) {
        if (tile == walkable[i]) return 1;
    }
    return 0;
}

static void classify_tiles(void) {
    const uint8_t *tiles = ZELDA_PLAY_AREA_TILES;
    int overworld = g_ram[0x0010] == 0; /* CurLevel */
    uint8_t first_unwalkable = g_ram[0x034A];
    memset(s_heights, 0, sizeof(s_heights));

    for (int x = 0; x < ZELDA_TILE_COLUMNS; x++) {
        for (int y = 0; y < ZELDA_TILE_ROWS; y++) {
            int index = tile_index(x, y);
            uint8_t tile = tiles[index];
            if (overworld && tile >= 0x8D && tile < 0x99) {
                s_classification[index] = 2; /* water/recess */
                s_heights[index] = -2.0f;
            } else if ((overworld && overworld_walkable_exception(tile)) ||
                       tile < first_unwalkable) {
                s_classification[index] = 0; /* walkable ground */
            } else {
                s_classification[index] = 1; /* connected raised structure */
            }
        }
    }

    /* Give each connected collision component one coherent elevation.  This
     * turns houses, rock formations, tree lines, and dungeon walls into solid
     * shapes without baking room-specific data into the engine. */
    for (int start = 0; start < ZELDA_TILE_COUNT; start++) {
        int head = 0, tail = 0;
        float height;
        if (s_classification[start] != 1 || s_heights[start] != 0.0f) continue;
        s_queue[tail++] = start;
        s_heights[start] = -1000.0f; /* visited sentinel */
        while (head < tail) {
            int index = s_queue[head++];
            int x = index / ZELDA_TILE_ROWS;
            int y = index % ZELDA_TILE_ROWS;
            const int dx[4] = { -1, 1, 0, 0 };
            const int dy[4] = { 0, 0, -1, 1 };
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d], next;
                if (nx < 0 || nx >= ZELDA_TILE_COLUMNS ||
                    ny < 0 || ny >= ZELDA_TILE_ROWS)
                    continue;
                next = tile_index(nx, ny);
                if (s_classification[next] == 1 && s_heights[next] == 0.0f) {
                    s_heights[next] = -1000.0f;
                    s_queue[tail++] = next;
                }
            }
        }
        if (tail >= 96) height = 28.0f;
        else if (tail >= 32) height = 22.0f;
        else if (tail >= 12) height = 17.0f;
        else if (tail >= 4) height = 13.0f;
        else height = 9.0f;
        for (int i = 0; i < tail; i++) s_heights[s_queue[i]] = height;
    }
}

static float zelda_tile_height(uint8_t tile, int x, int y, void *user) {
    (void)tile;
    (void)user;
    return s_heights[tile_index(x, y)];
}

static int angle_index_from_value(const char *value) {
    int requested = value ? atoi(value) : 35;
    int best = 1;
    int best_distance = abs(requested - s_angles[best]);
    for (int i = 2; i < (int)(sizeof(s_angles) / sizeof(s_angles[0])); i++) {
        int distance = abs(requested - s_angles[i]);
        if (distance < best_distance) {
            best = i;
            best_distance = distance;
        }
    }
    return best;
}

void zelda_voxel_configure_arg(const char *value) {
    s_requested_at_startup = 1;
    s_angle_index = angle_index_from_value(value);
}

void zelda_voxel_init(void) {
    if (s_requested_at_startup) {
        g_render_width = ZELDA_PLAYFIELD_WIDTH + ZELDA_WIDE_MARGIN * 2;
        g_widescreen_left = ZELDA_WIDE_MARGIN;
        g_widescreen_right = ZELDA_WIDE_MARGIN;
        printf("[Voxel] Zelda diorama enabled at %d degrees (key 3 cycles views)\n",
               s_angles[s_angle_index]);
    }
}

void zelda_voxel_update_hotkey(void) {
    int down = 0;
    if (SDL_WasInit(SDL_INIT_VIDEO)) {
        const uint8_t *keys = SDL_GetKeyboardState(NULL);
        down = keys && keys[SDL_SCANCODE_3];
    }
    if (down && !s_key_was_down) {
        s_angle_index =
            (s_angle_index + 1) % (int)(sizeof(s_angles) / sizeof(s_angles[0]));
        printf("[Voxel] view %s",
               s_angle_index ? "enabled" : "disabled");
        if (s_angle_index) printf(" at %d degrees", s_angles[s_angle_index]);
        printf("\n");
    }
    s_key_was_down = down;

    /* Keep menus and title screens centered instead of exposing wrapped
     * nametable content in the fixed 16:9 framebuffer. */
    if (s_requested_at_startup) {
        int margin = s_angle_index && gameplay_scene_visible()
            ? ZELDA_WIDE_MARGIN : 0;
        g_ws_eff_left = margin;
        g_ws_eff_right = margin;
    }
}

static int gameplay_scene_visible(void) {
    uint8_t mode = g_ram[0x0012]; /* GameMode */
    int playing = mode == 5 || mode == 9 || mode == 11 || mode == 12;
    if (!playing) return 0;
    if (g_ram[0x00E1] != 0) return 0; /* MenuState: preserve sliding inventory */
    return 1;
}

void zelda_voxel_post_render(uint32_t *framebuffer) {
    NesVoxelScene scene;
    if (!s_angle_index || !gameplay_scene_visible()) return;

    classify_tiles();
    memset(&scene, 0, sizeof(scene));
    scene.framebuffer = framebuffer;
    scene.output_width = g_render_width;
    scene.output_height = 240;
    scene.source_x = g_widescreen_left + ZELDA_PLAYFIELD_X;
    scene.source_y = ZELDA_PLAYFIELD_Y;
    scene.source_width = ZELDA_PLAYFIELD_WIDTH;
    scene.source_height = ZELDA_PLAYFIELD_HEIGHT;
    scene.tiles = ZELDA_PLAY_AREA_TILES;
    scene.tile_columns = ZELDA_TILE_COLUMNS;
    scene.tile_rows = ZELDA_TILE_ROWS;
    scene.tile_stride = ZELDA_TILE_ROWS;
    scene.column_major = 1;
    scene.tile_size = 8;
    scene.tile_height = zelda_tile_height;
    scene.elevation_degrees = (float)s_angles[s_angle_index];
    scene.draw_oam_sprites = 1;
    scene.preserve_top_rows = ZELDA_PLAYFIELD_Y;
    scene.extend_preserved_rows = 1;
    scene.preserved_rows_fill = 0xFF000000u;
    scene.sky_top = 0xFF7EB8E8u;
    scene.sky_bottom = 0xFFE5F0CBu;
    nes_voxel_render(&scene);
}
