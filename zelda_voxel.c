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
static int clamp_int(int value, int low, int high);

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
        for (int i = 0; i < tail; i++) heights[s_queue[i]] = height;
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
    s_mod_enabled = enabled != 0;
    s_view_enabled = s_mod_enabled;
    if (!s_mod_enabled) {
        s_stable_frame_valid = 0;
        s_was_scrolling = 0;
        s_exit_loading = 0;
        s_room_snapshot_valid = 0;
    }
}

void zelda_voxel_configure_mod(int pitch, int yaw, int roll,
                               int zoom_percent, int sprite_scale_percent) {
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
    if (!s_mod_enabled || !event || event->type != SDL_KEYDOWN)
        return;
    key = event->key.keysym.scancode;
    /* Adjustment keys honor SDL repeat so holding a numpad direction sweeps
     * the target. Keep the toggle and reset edge-triggered. */
    if (event->key.repeat &&
        (key == SDL_SCANCODE_KP_0 || key == SDL_SCANCODE_KP_5))
        return;
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

static float ease_control(float current, float target) {
    float delta = target - current;
    if (delta > -0.05f && delta < 0.05f) return target;
    return current + delta * 0.25f;
}

static void update_render_controls(void) {
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
    /* Keep the room behind Link's spin/fade/spark, then hand submodes B/C
     * back to the native renderer when the GAME OVER text is queued. */
    int death_animation = mode == 0x11 && submode <= 0x0A;
    int playing =
        mode == 5 || mode == 9 || mode == 0x0A || mode == 11 || mode == 12 ||
        mode == 0x10 || mode == 4 || world_unfurl_active() ||
        death_animation;
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
    scene.sprite_face_camera_pitch = 1;
    scene.sprite_constant_screen_size = 1;
    scene.sprite_depth_bias = 1.0f;
    scene.sprite_ground = zelda_sprite_ground;
    scene.sprite_overlay = zelda_sprite_overlay;
    scene.draw_oam_sprites = !world_unfurl_active();
    scene.preserve_top_rows = ZELDA_PLAYFIELD_Y;
    scene.extend_preserved_rows = 1;
    scene.preserved_rows_fill = 0xFF000000u;
    configure_scene_backdrop(&scene);
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
