/*
 * zelda_voxel.c -- Legend of Zelda profile for NESRecomp's voxel compositor
 *
 * The renderer remains presentation-only.  This profile derives geometry from
 * Zelda's live 32x22 PlayAreaTiles array and the same walkability boundary used
 * by the game.  No ROM assets are distributed or replaced.
 */
#include "zelda_voxel.h"

#include "config.h"
#include "controller.h"
#include "keybinds.h"
#include "nes_runtime.h"
#include "voxel_renderer.h"

#include <SDL.h>
#include <math.h>
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
#define ZELDA_ATTR_COLUMNS      8
#define ZELDA_ATTR_ROWS         6
#define ZELDA_ATTR_COUNT        (ZELDA_ATTR_COLUMNS * ZELDA_ATTR_ROWS)
#define ZELDA_TRANSITION_COLUMNS (ZELDA_TILE_COLUMNS * 2)
#define ZELDA_TRANSITION_ROWS    (ZELDA_TILE_ROWS * 2)
#define ZELDA_TRANSITION_COUNT \
    (ZELDA_TRANSITION_COLUMNS * ZELDA_TRANSITION_ROWS)
#define ZELDA_WIDE_MARGIN       85
#define ZELDA_OUTPUT_WIDTH      (ZELDA_PLAYFIELD_WIDTH + ZELDA_WIDE_MARGIN * 2)
#define ZELDA_OUTPUT_HEIGHT     240
#define ZELDA_PI                3.14159265358979323846f

/* CPU $6530 maps to g_sram[$0530].  Columns are contiguous 22-byte runs. */
#define ZELDA_PLAY_AREA_TILES   (g_sram + 0x0530)
#define ZELDA_PLAY_AREA_ATTRS   (g_ram + 0x0530)

static float s_heights[ZELDA_TILE_COUNT];
static uint8_t s_classification[ZELDA_TILE_COUNT];
static int s_queue[ZELDA_TILE_COUNT];
static uint8_t s_room_tiles[ZELDA_TILE_COUNT];
static uint8_t s_room_attrs[ZELDA_ATTR_COUNT];
static float s_room_heights[ZELDA_TILE_COUNT];
static uint8_t s_transition_tiles[ZELDA_TRANSITION_COUNT];
static uint8_t s_transition_palettes[ZELDA_TRANSITION_COUNT];
static float s_transition_heights[ZELDA_TRANSITION_COUNT];
static int s_transition_columns;
static int s_transition_rows;
static int s_room_snapshot_valid;
static uint32_t s_stable_frame[ZELDA_OUTPUT_WIDTH * ZELDA_OUTPUT_HEIGHT];
static int s_stable_frame_valid;
static int s_was_scrolling;
static int s_exit_loading;
static int s_mod_enabled;
static int s_view_enabled;
static int s_first_person;
static float s_first_person_heading = -90.0f;
static float s_first_person_look_pitch;
static int s_first_person_heading_initialized;
static float s_left_stick_x;
static float s_left_stick_y;
static float s_right_stick_x;
static float s_right_stick_y;
static float s_diorama_yaw_remainder;
static float s_diorama_pitch_remainder;
static int s_first_person_last_mapped_direction;
static uint8_t s_first_person_raw_buttons;
static uint64_t s_first_person_raw_buttons_frame = UINT64_MAX;
static int s_first_person_input_overridden;
static int s_pitch = 35;
static int s_yaw = -20;
static int s_roll;
static int s_zoom = 100;
static int s_sprite_scale = 135;
static float s_render_pitch = 35.0f;
static float s_render_yaw = -20.0f;
static float s_render_roll;
static float s_render_zoom = 100.0f;
static float s_render_sprite_scale = 135.0f;
static int s_default_pitch = 35;
static int s_default_yaw = -20;
static int s_default_roll;
static int s_default_zoom = 100;
static int s_default_sprite_scale = 135;

static int gameplay_scene_visible(void);
static int scrolling_scene_visible(void);
static int world_load_scene_visible(void);
static int world_unfurl_active(void);
static int dungeon_exit_scroll_pending(void);
static int current_tiles_are_overworld(void);
static void configure_scene_backdrop(NesVoxelScene *scene);
static void update_render_controls(void);
static void update_first_person_controls(void);
static void restore_first_person_input(void);
static int clamp_int(int value, int low, int high);
static float clamp_float(float value, float low, float high);

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

static int overworld_tree_tile(uint8_t tile) {
    /* Zelda's two scattered-tree metatile families occupy C4-C7 and C8-CB.
     * The later CE-DF patterns are solid boundary rocks and intentionally
     * remain extruded terrain. */
    return tile >= 0xC4 && tile <= 0xCB;
}

static int overworld_tree_group_member(const uint8_t *tiles, int x, int y) {
    int anchor_x = x & ~1;
    int anchor_y = y & ~1;
    if (anchor_x + 1 >= ZELDA_TILE_COLUMNS ||
        anchor_y + 1 >= ZELDA_TILE_ROWS)
        return 0;
    for (int group_x = 0; group_x < 2; group_x++)
        for (int group_y = 0; group_y < 2; group_y++)
            if (!overworld_tree_tile(
                    tiles[tile_index(anchor_x + group_x,
                                     anchor_y + group_y)]))
                return 0;
    return 1;
}

static int current_tiles_are_overworld(void) {
    uint8_t mode = g_ram[0x0012];
    uint8_t submode = g_ram[0x0013];
    if (g_ram[0x0010] != 0) return 0; /* CurLevel */
    if ((mode == 0x0B || mode == 0x0C) &&
        (submode >= 4 || g_ram[0x0011] != 0))
        return 0;
    if (mode == 0x0A && g_ram[0x005A] != 0 && submode < 5)
        return 0;
    return 1;
}

static void classify_tiles_into(const uint8_t *tiles, float *heights) {
    int overworld = current_tiles_are_overworld();
    uint8_t first_unwalkable = g_ram[0x034A];
    memset(heights, 0, sizeof(float) * ZELDA_TILE_COUNT);

    for (int x = 0; x < ZELDA_TILE_COLUMNS; x++) {
        for (int y = 0; y < ZELDA_TILE_ROWS; y++) {
            int index = tile_index(x, y);
            uint8_t tile = tiles[index];
            if (overworld && tile >= 0x8D && tile < 0x99) {
                s_classification[index] = 2; /* water/recess */
                heights[index] = -2.0f;
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
        if (s_classification[start] != 1 || heights[start] != 0.0f) continue;
        s_queue[tail++] = start;
        heights[start] = -1000.0f; /* visited sentinel */
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
                if (s_classification[next] == 1 && heights[next] == 0.0f) {
                    heights[next] = -1000.0f;
                    s_queue[tail++] = next;
                }
            }
        }
        if (tail >= 96) height = 28.0f;
        else if (tail >= 32) height = 22.0f;
        else if (tail >= 12) height = 17.0f;
        else if (tail >= 4) height = 13.0f;
        else height = 9.0f;
        for (int i = 0; i < tail; i++) {
            int index = s_queue[i];
            /* Forest crowns are rendered below as camera-facing cards. Their
             * collision footprint remains flat presentation geometry rather
             * than a square prism. */
            heights[index] =
                overworld && overworld_tree_group_member(
                    tiles, index / ZELDA_TILE_ROWS,
                    index % ZELDA_TILE_ROWS)
                    ? 0.0f : height;
        }
    }
}

static void classify_tiles(void) {
    classify_tiles_into(ZELDA_PLAY_AREA_TILES, s_heights);
}

static float zelda_tile_height(uint8_t tile, int x, int y, void *user) {
    (void)tile;
    (void)user;
    return s_heights[tile_index(x, y)];
}

static int zelda_sprite_overlay(int min_x, int min_y,
                                int max_x, int max_y, void *user) {
    (void)min_x;
    (void)min_y;
    (void)max_x;
    (void)max_y;
    (void)user;
    /* Respect terrain depth. Forcing actors over foreground walls makes a
     * top-down overlap look like they teleported onto their roofs. */
    return 0;
}

static int zelda_first_person_sprite_visible(int min_x, int min_y,
                                              int max_x, int max_y,
                                              void *user) {
    int player_x = g_ram[0x0070]; /* ObjX[0] */
    int player_y = g_ram[0x0084]; /* ObjY[0] */
    (void)user;
    /* The camera occupies Link's body. Suppress only the connected metasprite
     * around his live object coordinates; enemies, weapons, pickups, and
     * effects remain reconstructed as cards. */
    if (max_x >= player_x - 4 && min_x <= player_x + 20 &&
        max_y >= player_y - 4 && min_y <= player_y + 24)
        return 0;
    return 1;
}

static float zelda_sprite_ground(int min_x, int min_y,
                                 int max_x, int max_y,
                                 float sampled_ground, void *user) {
    (void)min_x;
    (void)min_y;
    (void)max_x;
    (void)max_y;
    (void)sampled_ground;
    (void)user;
    /* Zelda has no traversable raised layers. Actors can overlap a wall in the
     * top-down source image, but Link, enemies, pickups, and effects all remain
     * on the playfield floor rather than teleporting onto an extruded roof. */
    return 0.0f;
}

static int zelda_sprite_connect(int first_index, int second_index,
                                void *user) {
    int first = first_index * 4;
    int second = second_index * 4;
    uint8_t first_tile = g_ppu_oam[first + 1];
    uint8_t second_tile = g_ppu_oam[second + 1];
    uint8_t first_attr = g_ppu_oam[first + 2];
    uint8_t second_attr = g_ppu_oam[second + 2];
    int first_x = g_ppu_oam[first + 3];
    int second_x = g_ppu_oam[second + 3];
    int first_y = g_ppu_oam[first] + 1;
    int second_y = g_ppu_oam[second] + 1;
    (void)user;

    /* Tektites store their left and right 8-pixel halves in two distant OAM
     * bands rather than consecutively. They share a CA/CC animation tile,
     * palette and row, sit exactly eight pixels apart, and mirror horizontally.
     * Join only that signature so nearby independent enemies remain separate. */
    if (first_tile != second_tile ||
        (first_tile != 0xCA && first_tile != 0xCC))
        return 0;
    if (first_y != second_y || abs(first_x - second_x) != 8)
        return 0;
    if ((first_attr & 0x83) != (second_attr & 0x83))
        return 0;
    return ((first_attr ^ second_attr) & 0x40) != 0;
}

static int zelda_tree_billboard(uint8_t tile, int x, int y,
                                int *columns, int *rows, void *user) {
    const NesVoxelScene *scene = (const NesVoxelScene *)user;
    int anchor_x = x & ~1;
    int anchor_y = y & ~1;
    if (!current_tiles_are_overworld() || !overworld_tree_tile(tile))
        return 0;
    if (!scene || anchor_x + 1 >= scene->tile_columns ||
        anchor_y + 1 >= scene->tile_rows)
        return 0;
    for (int group_y = 0; group_y < 2; group_y++) {
        for (int group_x = 0; group_x < 2; group_x++) {
            int tile_x = anchor_x + group_x;
            int tile_y = anchor_y + group_y;
            int index = scene->column_major
                ? tile_x * scene->tile_stride + tile_y
                : tile_y * scene->tile_stride + tile_x;
            if (!overworld_tree_tile(scene->tiles[index]))
                return 0;
        }
    }
    /* Zelda's outdoor trees are 16x16 metatiles made from four 8x8 CHR
     * patterns. Even/even is the crown's top-left; the other three cells are
     * suppressed as terrain and consumed by that one card. */
    if ((x & 1) || (y & 1)) return -1;
    *columns = 2;
    *rows = 2;
    return 1;
}

static int zelda_sprite_max_height(const int *members, int member_count,
                                   void *user) {
    (void)user;
    for (int i = 0; i < member_count; i++) {
        uint8_t tile = g_ppu_oam[members[i] * 4 + 1];
        /* Link's animation patterns live in the leading sprite tile range.
         * Doorway-only OAM pieces can touch his normal horizontal pair and
         * must not turn it into a 16x32 card. */
        if (tile < 0x20) return 16;
    }
    return 0;
}

static float zelda_sprite_shadow(int min_x, int min_y,
                                 int max_x, int max_y, void *user) {
    (void)min_x;
    (void)min_y;
    (void)max_x;
    (void)max_y;
    (void)user;
    /* Every reconstructed OAM card gets a proportional contact shadow,
     * including pickups, weapons, projectiles, and transient effects. */
    return 1.0f;
}

static uint8_t room_tile_palette(const uint8_t *attrs, int x, int y) {
    uint8_t attr = attrs[(y / 4) * ZELDA_ATTR_COLUMNS + x / 4];
    int shift = ((x & 2) ? 2 : 0) + ((y & 2) ? 4 : 0);
    return (attr >> shift) & 3;
}

static void draw_tile_pixels(uint32_t *pixels, int stride, uint8_t tile,
                             int palette) {
    int chr_base = (g_ppuctrl & 0x10) ? 0x1000 : 0;
    for (int py = 0; py < 8; py++) {
        uint8_t lo = g_chr_ram[chr_base + tile * 16 + py];
        uint8_t hi = g_chr_ram[chr_base + tile * 16 + py + 8];
        for (int px = 0; px < 8; px++) {
            int bit = 7 - px;
            int color_index =
                ((lo >> bit) & 1) | (((hi >> bit) & 1) << 1);
            uint8_t nes_color =
                g_ppu_pal[(palette + color_index) & 0x1F] & 0x3F;
            pixels[py * stride + px] = g_nes_palette[nes_color];
        }
    }
}

static void zelda_room_tile_pixels(uint32_t *pixels, int stride,
                                   uint8_t tile, int x, int y, void *user) {
    int palette = room_tile_palette(ZELDA_PLAY_AREA_ATTRS, x, y) * 4;
    (void)user;
    draw_tile_pixels(pixels, stride, tile, palette);
}

static void copy_transition_room(int dst_x, int dst_y,
                                 const uint8_t *tiles,
                                 const uint8_t *attrs,
                                 const float *heights) {
    for (int x = 0; x < ZELDA_TILE_COLUMNS; x++) {
        for (int y = 0; y < ZELDA_TILE_ROWS; y++) {
            int source = tile_index(x, y);
            int dest = (dst_y + y) * s_transition_columns + dst_x + x;
            s_transition_tiles[dest] = tiles[source];
            s_transition_palettes[dest] = room_tile_palette(attrs, x, y);
            s_transition_heights[dest] = heights[source];
        }
    }
}

static float zelda_transition_tile_height(uint8_t tile, int x, int y,
                                          void *user) {
    (void)tile;
    (void)user;
    return s_transition_heights[y * s_transition_columns + x];
}

static void zelda_transition_tile_pixels(uint32_t *pixels, int stride,
                                         uint8_t tile, int x, int y,
                                         void *user) {
    int palette =
        s_transition_palettes[y * s_transition_columns + x] * 4;
    (void)user;
    draw_tile_pixels(pixels, stride, tile, palette);
}

static int transition_progress_pixels(int direction) {
    int submode = g_ram[0x0013];
    int progress = 0;
    if (submode < 3) return 0;
    if (submode > 3)
        return direction & 3 ? ZELDA_PLAYFIELD_WIDTH
                             : ZELDA_PLAYFIELD_HEIGHT;

    if (direction == 1) { /* right */
        progress = g_ram[0x00FD];
    } else if (direction == 2) { /* left */
        int scroll = g_ram[0x00FD];
        progress = scroll ? 256 - scroll : 0;
    } else if (direction == 4) { /* down */
        int row = g_ram[0x00E9];
        progress = row < ZELDA_TILE_ROWS ? (row + 1) * 8 : 0;
    } else if (direction == 8) { /* up */
        int row = g_ram[0x00E9];
        progress = row < ZELDA_TILE_ROWS
            ? (ZELDA_TILE_ROWS - row) * 8 : 0;
    }

    {
        int limit = direction & 3 ? ZELDA_PLAYFIELD_WIDTH
                                  : ZELDA_PLAYFIELD_HEIGHT;
        return clamp_int(progress, 0, limit);
    }
}

static int prepare_transition_scene(NesVoxelScene *scene) {
    const uint8_t *new_tiles = ZELDA_PLAY_AREA_TILES;
    const uint8_t *new_attrs = ZELDA_PLAY_AREA_ATTRS;
    int direction = g_ram[0x0098] & 0x0F; /* ObjDir */
    int progress, viewport_x = 0, viewport_y = 0;
    int old_x = 0, old_y = 0, new_x = 0, new_y = 0;

    if (!s_room_snapshot_valid ||
        (direction != 1 && direction != 2 &&
         direction != 4 && direction != 8))
        return 0;

    classify_tiles_into(new_tiles, s_heights);
    if (direction & 3) {
        s_transition_columns = ZELDA_TRANSITION_COLUMNS;
        s_transition_rows = ZELDA_TILE_ROWS;
        if (direction == 1) {
            new_x = ZELDA_TILE_COLUMNS;
        } else {
            new_x = 0;
            old_x = ZELDA_TILE_COLUMNS;
        }
    } else {
        s_transition_columns = ZELDA_TILE_COLUMNS;
        s_transition_rows = ZELDA_TRANSITION_ROWS;
        if (direction == 4) {
            new_y = ZELDA_TILE_ROWS;
        } else {
            new_y = 0;
            old_y = ZELDA_TILE_ROWS;
        }
    }

    memset(s_transition_tiles, 0,
           (size_t)s_transition_columns * s_transition_rows);
    memset(s_transition_palettes, 0,
           (size_t)s_transition_columns * s_transition_rows);
    memset(s_transition_heights, 0,
           sizeof(float) * (size_t)s_transition_columns * s_transition_rows);
    copy_transition_room(old_x, old_y, s_room_tiles, s_room_attrs,
                         s_room_heights);
    copy_transition_room(new_x, new_y, new_tiles, new_attrs, s_heights);

    progress = transition_progress_pixels(direction);
    if (direction == 1) viewport_x = progress;
    if (direction == 2) viewport_x = ZELDA_PLAYFIELD_WIDTH - progress;
    if (direction == 4) viewport_y = progress;
    if (direction == 8) viewport_y = ZELDA_PLAYFIELD_HEIGHT - progress;

    scene->tiles = s_transition_tiles;
    scene->tile_columns = s_transition_columns;
    scene->tile_rows = s_transition_rows;
    scene->tile_stride = s_transition_columns;
    scene->column_major = 0;
    scene->tile_height = zelda_transition_tile_height;
    scene->tile_pixels = zelda_transition_tile_pixels;
    scene->use_camera_target = 1;
    scene->camera_target_x =
        (float)viewport_x + ZELDA_PLAYFIELD_WIDTH * 0.5f;
    scene->camera_target_z =
        (float)viewport_y + ZELDA_PLAYFIELD_HEIGHT * 0.5f;
    scene->sprite_world_offset_x = (float)viewport_x;
    scene->sprite_world_offset_z = (float)viewport_y;
    return 1;
}

void zelda_voxel_set_mod_enabled(int enabled) {
    if (!enabled)
        restore_first_person_input();
    s_mod_enabled = enabled != 0;
    s_view_enabled = s_mod_enabled;
    if (!s_mod_enabled) {
        s_stable_frame_valid = 0;
        s_was_scrolling = 0;
        s_exit_loading = 0;
        s_room_snapshot_valid = 0;
    }
}

void zelda_voxel_configure_mod(int first_person,
                               int pitch, int yaw, int roll,
                               int zoom_percent, int sprite_scale_percent) {
    restore_first_person_input();
    s_first_person = first_person != 0;
    s_first_person_heading_initialized = 0;
    s_first_person_look_pitch = 0.0f;
    s_left_stick_x = s_left_stick_y = 0.0f;
    s_right_stick_x = s_right_stick_y = 0.0f;
    s_diorama_yaw_remainder = s_diorama_pitch_remainder = 0.0f;
    s_first_person_last_mapped_direction = 0;
    s_first_person_raw_buttons_frame = UINT64_MAX;
    s_pitch = s_default_pitch = pitch;
    s_yaw = s_default_yaw = yaw;
    s_roll = s_default_roll = roll;
    s_zoom = s_default_zoom = zoom_percent;
    s_sprite_scale = s_default_sprite_scale = sprite_scale_percent;
    s_render_pitch = (float)pitch;
    s_render_yaw = (float)yaw;
    s_render_roll = (float)roll;
    s_render_zoom = (float)zoom_percent;
    s_render_sprite_scale = (float)sprite_scale_percent;
}

void zelda_voxel_init(void) {
    if (s_mod_enabled) {
        g_render_width = ZELDA_OUTPUT_WIDTH;
        g_widescreen_left = ZELDA_WIDE_MARGIN;
        g_widescreen_right = ZELDA_WIDE_MARGIN;
        printf("[Voxel] Zelda %s enabled: pitch=%d yaw=%d roll=%d "
               "zoom=%d%% sprites=%d%% (numpad adjusts)\n",
               s_first_person ? "first-person" : "overworld diorama",
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
    int action;
    SDL_Scancode key;
    if (!s_mod_enabled || !event)
        return;
    if (event->type == SDL_CONTROLLERAXISMOTION &&
        g_nes_config.player_src[0] == 2 &&
        controller_instance_is_player(event->caxis.which, 1)) {
        float value = event->caxis.value < 0
            ? (float)event->caxis.value / 32768.0f
            : (float)event->caxis.value / 32767.0f;
        switch (event->caxis.axis) {
            case SDL_CONTROLLER_AXIS_LEFTX:
                if (s_first_person) s_left_stick_x = value;
                break;
            case SDL_CONTROLLER_AXIS_LEFTY:
                if (s_first_person) s_left_stick_y = value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTX: s_right_stick_x = value; break;
            case SDL_CONTROLLER_AXIS_RIGHTY: s_right_stick_y = value; break;
            default: break;
        }
        return;
    }
    if (event->type == SDL_CONTROLLERDEVICEREMOVED) {
        s_left_stick_x = s_left_stick_y = 0.0f;
        s_right_stick_x = s_right_stick_y = 0.0f;
        return;
    }
    if (event->type != SDL_KEYDOWN)
        return;
    key = event->key.keysym.scancode;
    action = keybinds_camera_action_for_scancode(key);
    /* Adjustment keys honor SDL repeat so holding a numpad direction sweeps
     * the target. Keep the toggle and reset edge-triggered. */
    if (event->key.repeat &&
        (action == NES_CAMERA_TOGGLE || action == NES_CAMERA_RESET))
        return;
    switch (action) {
        case NES_CAMERA_TOGGLE:
            s_view_enabled = !s_view_enabled;
            if (!s_view_enabled)
                restore_first_person_input();
            changed = 1;
            break;
        case NES_CAMERA_LOOK_UP:
            s_pitch = clamp_int(s_pitch + 5,
                                s_first_person ? -30 : 5,
                                s_first_person ? 30 : 85);
            changed = 1;
            break;
        case NES_CAMERA_LOOK_DOWN:
            s_pitch = clamp_int(s_pitch - 5,
                                s_first_person ? -30 : 5,
                                s_first_person ? 30 : 85);
            changed = 1;
            break;
        case NES_CAMERA_LOOK_LEFT:
            s_yaw = clamp_int(s_yaw - 5, -180, 180); changed = 1; break;
        case NES_CAMERA_LOOK_RIGHT:
            s_yaw = clamp_int(s_yaw + 5, -180, 180); changed = 1; break;
        case NES_CAMERA_ROLL_LEFT:
            s_roll = clamp_int(s_roll - 5, -45, 45); changed = 1; break;
        case NES_CAMERA_ROLL_RIGHT:
            s_roll = clamp_int(s_roll + 5, -45, 45); changed = 1; break;
        case NES_CAMERA_ZOOM_IN:
            s_zoom = clamp_int(s_zoom + 5, 50, 200); changed = 1; break;
        case NES_CAMERA_ZOOM_OUT:
            s_zoom = clamp_int(s_zoom - 5, 50, 200); changed = 1; break;
        case NES_CAMERA_SPRITE_SMALLER:
            s_sprite_scale =
                clamp_int(s_sprite_scale - 10, 75, 250); changed = 1; break;
        case NES_CAMERA_SPRITE_LARGER:
            s_sprite_scale =
                clamp_int(s_sprite_scale + 10, 75, 250); changed = 1; break;
        case NES_CAMERA_RESET:
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

static float ease_control(float current, float target) {
    float delta = target - current;
    if (delta > -0.05f && delta < 0.05f) return target;
    return current + delta * 0.25f;
}

static float first_person_heading_for_direction(int direction) {
    switch (direction) {
        case 1: return 0.0f;    /* right: +X */
        case 2: return 180.0f;  /* left:  -X */
        case 4: return 90.0f;   /* down:  +Z */
        default: return -90.0f; /* up:    -Z */
    }
}

static float wrap_heading(float heading) {
    while (heading > 180.0f) heading -= 360.0f;
    while (heading < -180.0f) heading += 360.0f;
    return heading;
}

static void initialize_first_person_heading(void) {
    if (!s_first_person_heading_initialized) {
        int object_direction = g_ram[0x0098] & 0x0F;
        if (object_direction != 1 && object_direction != 2 &&
            object_direction != 4 && object_direction != 8)
            object_direction = 8;
        s_first_person_heading =
            first_person_heading_for_direction(object_direction);
        s_first_person_heading_initialized = 1;
    }
}

static float stick_curve(float value, float deadzone) {
    float magnitude = fabsf(value);
    float scaled;
    if (magnitude <= deadzone) return 0.0f;
    scaled = (magnitude - deadzone) / (1.0f - deadzone);
    scaled *= scaled;
    return value < 0.0f ? -scaled : scaled;
}

static int first_person_controls_active(void) {
    return s_mod_enabled && s_view_enabled && s_first_person &&
           gameplay_scene_visible();
}

static void restore_first_person_input(void) {
    if (s_first_person_input_overridden)
        g_controller1_buttons = s_first_person_raw_buttons;
    s_first_person_input_overridden = 0;
    s_first_person_last_mapped_direction = 0;
}

static void update_first_person_look(void) {
    float yaw_input;
    float pitch_input;
    if (!first_person_controls_active()) return;
    initialize_first_person_heading();

    /* Match the modern free-look model used by Gen1Recomp's voxel mod:
     * the right stick owns attitude and never doubles as movement. A squared
     * response curve preserves fine aim near center without making a full
     * deflection feel sluggish. Values are degrees per 60 Hz frame. */
    yaw_input = stick_curve(s_right_stick_x, 0.18f);
    pitch_input = stick_curve(s_right_stick_y, 0.18f);
    s_first_person_heading = wrap_heading(
        s_first_person_heading + yaw_input * 3.5f);
    s_first_person_look_pitch = clamp_float(
        s_first_person_look_pitch - pitch_input * 2.4f, -50.0f, 50.0f);
}

static int quantize_camera_relative_direction(float world_x, float world_z) {
    float ax = fabsf(world_x);
    float az = fabsf(world_z);

    /* A small hysteresis band stops a held diagonal from rattling between
     * axes as the right stick crosses a 45-degree camera boundary. */
    if (fabsf(ax - az) < 0.10f &&
        s_first_person_last_mapped_direction != 0) {
        int last = s_first_person_last_mapped_direction;
        if (last == 1 || last == 2)
            return world_x >= 0.0f ? 1 : 2;
        return world_z >= 0.0f ? 4 : 8;
    }
    if (ax >= az)
        return world_x >= 0.0f ? 1 : 2;
    return world_z >= 0.0f ? 4 : 8;
}

static void remap_first_person_movement(void) {
    uint8_t raw_directions;
    uint8_t other_buttons;
    float strafe = 0.0f;
    float forward = 0.0f;
    float stick_magnitude =
        sqrtf(s_left_stick_x * s_left_stick_x +
              s_left_stick_y * s_left_stick_y);
    float heading;
    float world_x;
    float world_z;
    int mapped = 0;

    if (!first_person_controls_active()) {
        restore_first_person_input();
        return;
    }
    initialize_first_person_heading();
    /* game_on_frame may run more than once while resolving a nested VBlank.
     * Capture the runner's original byte once per presented frame so a second
     * pass never treats our already-remapped direction as fresh input. */
    if (s_first_person_raw_buttons_frame != g_frame_count) {
        s_first_person_raw_buttons = g_controller1_buttons;
        s_first_person_raw_buttons_frame = g_frame_count;
    }
    raw_directions = s_first_person_raw_buttons & 0x0F;
    other_buttons = s_first_person_raw_buttons & 0xF0;

    /* Prefer the left stick's true vector over the runner's already-quantized
     * d-pad bits. D-pad and keyboard remain useful camera-relative fallbacks. */
    if (stick_magnitude > 0.25f) {
        float strength =
            (stick_magnitude - 0.25f) / (1.0f - 0.25f);
        if (strength > 1.0f) strength = 1.0f;
        strafe = s_left_stick_x / stick_magnitude * strength;
        forward = -s_left_stick_y / stick_magnitude * strength;
    } else {
        strafe = ((raw_directions & 0x01) ? 1.0f : 0.0f) -
                 ((raw_directions & 0x02) ? 1.0f : 0.0f);
        forward = ((raw_directions & 0x08) ? 1.0f : 0.0f) -
                  ((raw_directions & 0x04) ? 1.0f : 0.0f);
    }

    if (fabsf(strafe) > 0.01f || fabsf(forward) > 0.01f) {
        /* Heading 0 faces world-right; +90 faces world-down. Forward follows
         * the view, and strafe-right is its clockwise ground-plane tangent. */
        heading =
            (s_first_person_heading + s_render_yaw) * ZELDA_PI / 180.0f;
        world_x = cosf(heading) * forward - sinf(heading) * strafe;
        world_z = sinf(heading) * forward + cosf(heading) * strafe;
        mapped = quantize_camera_relative_direction(world_x, world_z);
    }

    g_controller1_buttons = other_buttons | (uint8_t)mapped;
    s_first_person_input_overridden = 1;
    if (mapped != s_first_person_last_mapped_direction) {
        if (mapped != 0) {
            printf("[Voxel] Zelda first-person move heading=%.0f -> dir=%d\n",
                   wrap_heading(s_first_person_heading + s_render_yaw),
                   mapped);
        }
        s_first_person_last_mapped_direction = mapped;
    }
}

static void update_first_person_controls(void) {
    if (!s_first_person) return;
    update_first_person_look();
    remap_first_person_movement();
}

static void update_render_controls(void) {
    int whole;
    if (!s_first_person && s_mod_enabled && s_view_enabled) {
        s_diorama_yaw_remainder +=
            stick_curve(s_right_stick_x, 0.18f) * 2.5f;
        whole = (int)s_diorama_yaw_remainder;
        if (whole != 0) {
            s_yaw += whole;
            while (s_yaw > 180) s_yaw -= 360;
            while (s_yaw < -180) s_yaw += 360;
            s_diorama_yaw_remainder -= (float)whole;
        }
        s_diorama_pitch_remainder -=
            stick_curve(s_right_stick_y, 0.18f) * 2.0f;
        whole = (int)s_diorama_pitch_remainder;
        if (whole != 0) {
            s_pitch = clamp_int(s_pitch + whole, 5, 85);
            s_diorama_pitch_remainder -= (float)whole;
        }
    }
    s_render_pitch = ease_control(s_render_pitch, (float)s_pitch);
    s_render_yaw = ease_control(s_render_yaw, (float)s_yaw);
    s_render_roll = ease_control(s_render_roll, (float)s_roll);
    s_render_zoom = ease_control(s_render_zoom, (float)s_zoom);
    s_render_sprite_scale =
        ease_control(s_render_sprite_scale, (float)s_sprite_scale);
}

void zelda_voxel_update_hotkey(void) {
    if (!s_mod_enabled) return;
    update_render_controls();
    update_first_person_controls();
    /* Keep menus and title screens centered instead of exposing wrapped
     * nametable content in the fixed 16:9 framebuffer. */
    {
        int margin = s_view_enabled && gameplay_scene_visible()
            ? ZELDA_WIDE_MARGIN : 0;
        if (s_view_enabled && (scrolling_scene_visible() || s_was_scrolling))
            margin = ZELDA_WIDE_MARGIN;
        if (s_view_enabled && world_load_scene_visible())
            margin = ZELDA_WIDE_MARGIN;
        g_ws_eff_left = margin;
        g_ws_eff_right = margin;
    }
}

static int world_unfurl_active(void) {
    return g_ram[0x0012] == 3 && g_ram[0x0011] != 0;
}

static int world_load_scene_visible(void) {
    uint8_t mode = g_ram[0x0012];
    return mode == 2 || mode == 3 || mode == 4;
}

static int dungeon_exit_scroll_pending(void) {
    uint8_t mode = g_ram[0x0012];
    return (mode == 6 || mode == 7) && g_ram[0x0010] != 0 &&
           (g_ram[0x0098] & 0x0F) == 4 && /* ObjDir: down */
           g_ram[0x00EB] == g_sram[0x0BAD]; /* LevelInfo_StartRoomId */
}

static int gameplay_scene_visible(void) {
    uint8_t mode = g_ram[0x0012]; /* GameMode */
    uint8_t submode = g_ram[0x0013]; /* GameSubmode */
    /* Keep the room behind Link's spin/fade/spark, then hand death submodes
     * B/C back to the native renderer when the GAME OVER text is queued.
     * The end-level ceremony likewise belongs to the live dungeon scene. */
    int death_animation = mode == 0x11 && submode <= 0x0A;
    int end_level_animation = mode == 0x12;
    int playing =
        mode == 5 || mode == 9 || mode == 0x0A ||
        mode == 0x0B || mode == 0x0C || mode == 0x10 || mode == 4 ||
        world_unfurl_active() ||
        death_animation || end_level_animation;
    if (!playing) return 0;
    if (g_ram[0x00E1] != 0) return 0; /* MenuState: preserve sliding inventory */
    return 1;
}

static int scrolling_scene_visible(void) {
    uint8_t mode = g_ram[0x0012]; /* Mode 6 initializes, mode 7 scrolls. */
    return mode == 6 || mode == 7;
}

static void configure_scene_backdrop(NesVoxelScene *scene) {
    uint8_t mode = g_ram[0x0012];
    uint8_t submode = g_ram[0x0013];
    int underground =
        !current_tiles_are_overworld() || mode == 0x0B || mode == 0x0C;

    if (mode == 0x11 && submode >= 8) {
        /* Zelda fades the playfield to black before GAME OVER. Do not leave
         * the presentation-only outdoor sky glowing behind that fade. */
        scene->sky_top = 0xFF000000u;
        scene->sky_bottom = 0xFF000000u;
    } else if (underground) {
        /* Dungeon and cave rooms are enclosed spaces. A subdued surrounding
         * void keeps the projected edges from reading as outdoor daylight. */
        scene->sky_top = 0xFF05070Cu;
        scene->sky_bottom = 0xFF12100Eu;
    } else {
        scene->sky_top = 0xFF7EB8E8u;
        scene->sky_bottom = 0xFFE5F0CBu;
    }
}

static void fade_unfurl_from_black(uint32_t *framebuffer) {
    int column = clamp_int(g_ram[0x007C], 0, 16); /* ObjX+12 */
    int visible = 16 - column;
    for (int y = ZELDA_PLAYFIELD_Y; y < ZELDA_OUTPUT_HEIGHT; y++) {
        for (int x = 0; x < ZELDA_OUTPUT_WIDTH; x++) {
            uint32_t color =
                framebuffer[y * ZELDA_OUTPUT_WIDTH + x];
            unsigned r = ((color >> 16) & 0xFF) * visible / 16;
            unsigned g = ((color >> 8) & 0xFF) * visible / 16;
            unsigned b = (color & 0xFF) * visible / 16;
            framebuffer[y * ZELDA_OUTPUT_WIDTH + x] =
                0xFF000000u | (r << 16) | (g << 8) | b;
        }
    }
}

static float clamp_float(float value, float low, float high) {
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

static void configure_first_person_camera(NesVoxelScene *scene) {
    float heading;
    float look_pitch =
        (s_render_pitch + s_first_person_look_pitch) *
        ZELDA_PI / 180.0f;
    float look_distance = 128.0f;
    float eye_x;
    float eye_z;

    heading =
        (s_first_person_heading + s_render_yaw) * ZELDA_PI / 180.0f;

    eye_x = clamp_float((float)g_ram[0x0070] + 8.0f,
                        4.0f, ZELDA_PLAYFIELD_WIDTH - 4.0f);
    eye_z = clamp_float((float)g_ram[0x0084] -
                            ZELDA_PLAYFIELD_Y + 10.0f,
                        4.0f, ZELDA_PLAYFIELD_HEIGHT - 4.0f);
    eye_x += scene->sprite_world_offset_x + cosf(heading) * 2.0f;
    eye_z += scene->sprite_world_offset_z + sinf(heading) * 2.0f;

    scene->use_camera_pose = 1;
    scene->camera_eye_x = eye_x;
    scene->camera_eye_y = 8.0f;
    scene->camera_eye_z = eye_z;
    scene->camera_look_at_x =
        eye_x + cosf(heading) * cosf(look_pitch) * look_distance;
    scene->camera_look_at_y =
        scene->camera_eye_y + sinf(look_pitch) * look_distance;
    scene->camera_look_at_z =
        eye_z + sinf(heading) * cosf(look_pitch) * look_distance;
    scene->camera_focal_scale =
        clamp_float(0.78f * s_render_zoom / 100.0f, 0.42f, 1.35f);
    scene->camera_center_y = 0.61f;
}

void zelda_voxel_post_render(uint32_t *framebuffer) {
    NesVoxelScene scene;
    int gameplay_visible;
    int transition_visible;
    float roll_fit_percent;
    if (!s_mod_enabled || !s_view_enabled) return;
    gameplay_visible = gameplay_scene_visible();
    transition_visible = g_ram[0x0012] == 7;
    if (dungeon_exit_scroll_pending())
        s_exit_loading = 1;
    if (s_exit_loading &&
        (g_ram[0x0012] == 6 || transition_visible)) {
        /* Leaving a dungeon/cave does not scroll into an adjacent room. Mode
         * 7 is only a handoff into the overworld loader, whose partially
         * populated tile array must not replace the coherent indoor frame. */
        if (s_stable_frame_valid)
            memcpy(framebuffer, s_stable_frame, sizeof(s_stable_frame));
        s_was_scrolling = 1;
        return;
    }
    if (world_load_scene_visible() && !gameplay_visible) {
        /* World loading briefly leaves the previous nametable repeated across
         * the widescreen surface. Keep the coherent source diorama until the
         * destination room is laid out and its curtain animation can become a
         * 3D fade-in. A cold load has no source room, so use a clean blackout. */
        if (s_stable_frame_valid) {
            memcpy(framebuffer, s_stable_frame, sizeof(s_stable_frame));
        } else {
            memset(framebuffer, 0, sizeof(s_stable_frame));
        }
        return;
    }
    if (g_ram[0x0012] == 6 ||
        (s_was_scrolling && !transition_visible && !gameplay_visible)) {
        /* Mode 6 streams the destination room before native scrolling starts.
         * Keep the coherent source room during that hidden preparation, and
         * keep the final transition frame through mode 4's room handoff. */
        if (s_stable_frame_valid) {
            memcpy(framebuffer, s_stable_frame, sizeof(s_stable_frame));
        }
        if (!s_was_scrolling) {
            printf("[Voxel] room scroll: holding stable diorama (cache=%s)\n",
                   s_stable_frame_valid ? "ready" : "empty");
        }
        s_was_scrolling = 1;
        return;
    }
    if (!gameplay_visible && !transition_visible) {
        if (s_stable_frame_valid) {
            printf("[Voxel] scene mode %u invalidated stable diorama\n",
                   (unsigned)g_ram[0x0012]);
        }
        s_stable_frame_valid = 0;
        s_was_scrolling = 0;
        return;
    }

    memset(&scene, 0, sizeof(scene));
    scene.framebuffer = framebuffer;
    scene.output_width = g_render_width;
    scene.output_height = ZELDA_OUTPUT_HEIGHT;
    scene.source_x = g_widescreen_left + ZELDA_PLAYFIELD_X;
    scene.source_y = ZELDA_PLAYFIELD_Y;
    scene.source_width = ZELDA_PLAYFIELD_WIDTH;
    scene.source_height = ZELDA_PLAYFIELD_HEIGHT;
    scene.tile_size = 8;
    if (transition_visible) {
        if (!prepare_transition_scene(&scene)) {
            if (s_stable_frame_valid)
                memcpy(framebuffer, s_stable_frame, sizeof(s_stable_frame));
            s_was_scrolling = 1;
            return;
        }
    } else {
        classify_tiles();
        scene.tiles = ZELDA_PLAY_AREA_TILES;
        scene.tile_columns = ZELDA_TILE_COLUMNS;
        scene.tile_rows = ZELDA_TILE_ROWS;
        scene.tile_stride = ZELDA_TILE_ROWS;
        scene.column_major = 1;
        scene.tile_height = zelda_tile_height;
        if (world_unfurl_active() || g_ram[0x0012] == 0x0A)
            scene.tile_pixels = zelda_room_tile_pixels;
    }
    scene.elevation_degrees = s_render_pitch;
    scene.yaw_degrees = s_render_yaw;
    scene.roll_degrees = s_render_roll;
    /* A rolled rectangle needs more room than an axis-aligned one. Preserve
     * the user's zoom intent while automatically fitting the rotated playfield
     * inside the 426x240 presentation surface. */
    roll_fit_percent =
        100.0f + (s_render_roll < 0.0f ? -s_render_roll : s_render_roll);
    scene.camera_distance =
        285.0f * roll_fit_percent / s_render_zoom;
    scene.sprite_scale = s_render_sprite_scale / 100.0f;
    scene.user = &scene;
    if (current_tiles_are_overworld()) {
        scene.tile_billboard = zelda_tree_billboard;
        scene.tile_billboard_scale = s_first_person ? 1.0f : 1.35f;
        scene.tile_billboard_shadow_scale = 0.72f;
        scene.tile_billboard_shadow_opacity = 0.30f;
    }
    scene.sprite_face_camera_pitch = !s_first_person;
    scene.sprite_constant_screen_size = !s_first_person;
    scene.clip_sprites_to_source = 1;
    scene.sprite_depth_bias = s_first_person ? 0.2f : 1.0f;
    scene.sprite_ground = zelda_sprite_ground;
    scene.sprite_connect = zelda_sprite_connect;
    scene.sprite_max_height = zelda_sprite_max_height;
    scene.sprite_visible =
        s_first_person ? zelda_first_person_sprite_visible : NULL;
    scene.sprite_shadow = zelda_sprite_shadow;
    scene.sprite_shadow_scale = 0.62f;
    scene.sprite_shadow_opacity = 0.34f;
    scene.sprite_overlay = zelda_sprite_overlay;
    scene.draw_oam_sprites = !world_unfurl_active();
    scene.preserve_top_rows = ZELDA_PLAYFIELD_Y;
    scene.extend_preserved_rows = 1;
    scene.preserved_rows_fill = 0xFF000000u;
    configure_scene_backdrop(&scene);
    if (s_first_person)
        configure_first_person_camera(&scene);
    if (nes_voxel_render(&scene)) {
        if (world_unfurl_active())
            fade_unfurl_from_black(framebuffer);
        memcpy(s_stable_frame, framebuffer, sizeof(s_stable_frame));
        s_stable_frame_valid = 1;
        if (transition_visible) {
            s_was_scrolling = 1;
        } else {
            memcpy(s_room_tiles, ZELDA_PLAY_AREA_TILES,
                   sizeof(s_room_tiles));
            memcpy(s_room_attrs, ZELDA_PLAY_AREA_ATTRS,
                   sizeof(s_room_attrs));
            memcpy(s_room_heights, s_heights, sizeof(s_room_heights));
            s_room_snapshot_valid = 1;
        }
        if (s_was_scrolling && !transition_visible)
            printf("[Voxel] room scroll: destination diorama ready\n");
        if (!transition_visible) s_was_scrolling = 0;
        if (g_ram[0x0012] == 5)
            s_exit_loading = 0;
    }
}
