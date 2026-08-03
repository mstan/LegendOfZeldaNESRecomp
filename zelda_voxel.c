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

static float s_heights[ZELDA_TILE_COUNT];
static uint8_t s_classification[ZELDA_TILE_COUNT];
static int s_queue[ZELDA_TILE_COUNT];
static int s_mod_enabled;
static int s_view_enabled;
static int s_pitch = 35;
static int s_yaw = -20;
static int s_roll;
static int s_zoom = 100;
static int s_sprite_scale = 135;
static int s_default_pitch = 35;
static int s_default_yaw = -20;
static int s_default_roll;
static int s_default_zoom = 100;
static int s_default_sprite_scale = 135;

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

void zelda_voxel_set_mod_enabled(int enabled) {
    s_mod_enabled = enabled != 0;
    s_view_enabled = s_mod_enabled;
}

void zelda_voxel_configure_mod(int pitch, int yaw, int roll,
                               int zoom_percent, int sprite_scale_percent) {
    s_pitch = s_default_pitch = pitch;
    s_yaw = s_default_yaw = yaw;
    s_roll = s_default_roll = roll;
    s_zoom = s_default_zoom = zoom_percent;
    s_sprite_scale = s_default_sprite_scale = sprite_scale_percent;
}

void zelda_voxel_init(void) {
    if (s_mod_enabled) {
        g_render_width = ZELDA_PLAYFIELD_WIDTH + ZELDA_WIDE_MARGIN * 2;
        g_widescreen_left = ZELDA_WIDE_MARGIN;
        g_widescreen_right = ZELDA_WIDE_MARGIN;
        printf("[Voxel] Zelda diorama enabled: pitch=%d yaw=%d roll=%d "
               "zoom=%d%% sprites=%d%% (numpad adjusts)\n",
               s_pitch, s_yaw, s_roll, s_zoom, s_sprite_scale);
    }
}

static int clamp_int(int value, int low, int high) {
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

void zelda_voxel_handle_event(const SDL_Event *event) {
    int changed = 0;
    SDL_Scancode key;
    if (!s_mod_enabled || !event || event->type != SDL_KEYDOWN ||
        event->key.repeat)
        return;
    key = event->key.keysym.scancode;
    switch (key) {
        case SDL_SCANCODE_KP_0:
            s_view_enabled = !s_view_enabled; changed = 1; break;
        case SDL_SCANCODE_KP_8:
            s_pitch = clamp_int(s_pitch + 5, 5, 85); changed = 1; break;
        case SDL_SCANCODE_KP_2:
            s_pitch = clamp_int(s_pitch - 5, 5, 85); changed = 1; break;
        case SDL_SCANCODE_KP_4:
            s_yaw = clamp_int(s_yaw - 5, -180, 180); changed = 1; break;
        case SDL_SCANCODE_KP_6:
            s_yaw = clamp_int(s_yaw + 5, -180, 180); changed = 1; break;
        case SDL_SCANCODE_KP_7:
            s_roll = clamp_int(s_roll - 5, -45, 45); changed = 1; break;
        case SDL_SCANCODE_KP_9:
            s_roll = clamp_int(s_roll + 5, -45, 45); changed = 1; break;
        case SDL_SCANCODE_KP_PLUS:
            s_zoom = clamp_int(s_zoom + 5, 50, 200); changed = 1; break;
        case SDL_SCANCODE_KP_MINUS:
            s_zoom = clamp_int(s_zoom - 5, 50, 200); changed = 1; break;
        case SDL_SCANCODE_KP_1:
            s_sprite_scale =
                clamp_int(s_sprite_scale - 10, 75, 250); changed = 1; break;
        case SDL_SCANCODE_KP_3:
            s_sprite_scale =
                clamp_int(s_sprite_scale + 10, 75, 250); changed = 1; break;
        case SDL_SCANCODE_KP_5:
            s_pitch = s_default_pitch;
            s_yaw = s_default_yaw;
            s_roll = s_default_roll;
            s_zoom = s_default_zoom;
            s_sprite_scale = s_default_sprite_scale;
            s_view_enabled = 1;
            changed = 1;
            break;
        default:
            break;
    }
    if (changed) {
        printf("[Voxel] %s pitch=%d yaw=%d roll=%d zoom=%d%% sprites=%d%%\n",
               s_view_enabled ? "on" : "off",
               s_pitch, s_yaw, s_roll, s_zoom, s_sprite_scale);
    }
}

void zelda_voxel_update_hotkey(void) {
    if (!s_mod_enabled) return;
    /* Keep menus and title screens centered instead of exposing wrapped
     * nametable content in the fixed 16:9 framebuffer. */
    {
        int margin = s_view_enabled && gameplay_scene_visible()
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
    if (!s_mod_enabled || !s_view_enabled || !gameplay_scene_visible()) return;

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
    scene.elevation_degrees = (float)s_pitch;
    scene.yaw_degrees = (float)s_yaw;
    scene.roll_degrees = (float)s_roll;
    scene.camera_distance = 285.0f * 100.0f / (float)s_zoom;
    scene.sprite_scale = (float)s_sprite_scale / 100.0f;
    scene.draw_oam_sprites = 1;
    scene.preserve_top_rows = ZELDA_PLAYFIELD_Y;
    scene.extend_preserved_rows = 1;
    scene.preserved_rows_fill = 0xFF000000u;
    scene.sky_top = 0xFF7EB8E8u;
    scene.sky_bottom = 0xFFE5F0CBu;
    nes_voxel_render(&scene);
}
