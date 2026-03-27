/*
 * extras.c -- The Legend of Zelda game-specific runner hooks
 *
 * Implements game_extras.h for Legend of Zelda.
 * Features:
 *   TCP debug server (port 4370) gated behind debug.ini
 *   Verify/emulated mode via Nestopia oracle
 *   Entity debug diagnostics
 *   SRAM dispatch override for save file routines
 */
#include "game_extras.h"
#include "nes_runtime.h"
#include "debug_server.h"
#include "verify_mode.h"
#include "input_script.h"
#ifdef ENABLE_NESTOPIA_ORACLE
#include "nestopia_bridge.h"
#endif
#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

/* ---- Debug mode ---- */
static int s_debug_enabled = 0;
static void get_exe_relative_path(const char *filename, char *out, int max_len);

static int check_debug_ini(void) {
    char path[512];
    get_exe_relative_path("debug.ini", path, sizeof(path));
    FILE *f = fopen(path, "r");
    if (f) { fclose(f); return 1; }
    return 0;
}

/* ---- Debug server state ---- */
static int s_tcp_port = 4370;

/* ROM path exposed by runner for verify mode init */
const char *g_rom_path_for_extras = NULL;

/* ---- Path helper ---- */

/* Build path: <exe_dir>/filename. Same pattern as launcher.c:get_rom_cfg_path(). */
static void get_exe_relative_path(const char *filename, char *out, int max_len) {
#ifdef _WIN32
    char exe_path[MAX_PATH];
    GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    char *last_sep = strrchr(exe_path, '\\');
    if (last_sep) *(last_sep + 1) = '\0';
    snprintf(out, max_len, "%s%s", exe_path, filename);
#else
    snprintf(out, max_len, "%s", filename);
#endif
}

/* ---- game_extras.h implementation ---- */

uint32_t game_get_expected_crc32(void) { return 0xD7AE93DFu; }

const char *game_get_name(void) { return "The Legend of Zelda"; }

void game_on_init(void) {
    s_debug_enabled = check_debug_ini();

    if (s_debug_enabled) {
        printf("[Debug] debug.ini found -- TCP server and verify mode enabled\n");
        debug_server_init(s_tcp_port);

        if (g_run_mode != RUN_MODE_NATIVE && g_rom_path_for_extras) {
            verify_mode_init(g_rom_path_for_extras);
        }
    } else if (g_run_mode != RUN_MODE_NATIVE) {
        /* --verify or --emulated implies debug even without ini */
        s_debug_enabled = 1;
        debug_server_init(s_tcp_port);
        if (g_rom_path_for_extras)
            verify_mode_init(g_rom_path_for_extras);
    }
}

/* Dump the 10 entity slots: type, X, Y at $0300/$0380/$0390 */
static void dump_entities(uint64_t frame_count, const char *label) {
    printf("[%s] frame=%llu entity slots:\n", label, (unsigned long long)frame_count);
    for (int i = 0; i < 10; i++) {
        uint8_t type = g_ram[0x300 + i];
        uint8_t x    = g_ram[0x380 + i];
        uint8_t y    = g_ram[0x390 + i];
        if (type != 0)
            printf("  slot%d type=$%02X x=%d y=%d\n", i, type, x, y);
    }
    /* Also log $03F8 and $0394 used by sub-mode 8 */
    printf("  $0394=$%02X $03F8=$%02X $0F=$%02X\n",
           g_ram[0x394], g_ram[0x3F8], g_ram[0x0F]);
}

void game_on_frame(uint64_t frame_count) {
    if (s_debug_enabled) {
        debug_server_poll();
        debug_server_wait_if_paused();
        int ovr = debug_server_get_input_override();
        if (ovr >= 0)
            g_controller1_buttons = (uint8_t)ovr;
    }

    static uint8_t last_mode = 0xFF, last_sub = 0xFF;
    static int ob_sub_cycled = 0; /* did we see sub>0 in mode $0B? */
    uint8_t mode = g_ram[0x12], sub = g_ram[0x13];

    if (mode != last_mode || sub != last_sub) {
        printf("[Mode] frame=%llu mode=$%02X sub=$%02X\n",
               (unsigned long long)frame_count, mode, sub);

        /* When mode $0B init cycle completes (sub went >0 then back to 0) */
        if (mode == 0x0B && sub == 0x00 && ob_sub_cycled) {
            dump_entities(frame_count, "0B-done");
            ob_sub_cycled = 0;
        }
        if (mode == 0x0B && sub > 0x00)
            ob_sub_cycled = 1;

        last_mode = mode; last_sub = sub;
    }

    /* Entity diagnostics every 60 frames during gameplay or room-loaded state */
    if ((mode == 0x07 || mode == 0x06 || mode == 0x05) && (frame_count % 60) == 0) {
        dump_entities(frame_count, "Ent");
        printf("  $051E=$%02X $051F=$%02X $EB=$%02X\n",
               g_ram[0x51E], g_ram[0x51F], g_ram[0xEB]);
    }
    /* OAM shadow: first 8 sprites (32 bytes at $0200) */
    if (mode == 0x07 && (frame_count % 120) == 0) {
        printf("[OAM] f=%llu: ", (unsigned long long)frame_count);
        for (int i = 0; i < 8; i++) {
            uint8_t y  = g_ram[0x200 + i*4];
            uint8_t t  = g_ram[0x201 + i*4];
            uint8_t at = g_ram[0x202 + i*4];
            uint8_t x  = g_ram[0x203 + i*4];
            (void)at;
            printf("[%d]y=%d t=%02X x=%d ", i, y, t, x);
        }
        printf("\n");
    }
}

void game_post_nmi(uint64_t frame_count) {
    (void)frame_count;
    if (s_debug_enabled) {
        debug_server_record_frame();
        debug_server_check_watchpoints();
    }
}

int game_handle_arg(const char *key, const char *val) {
    if (strcmp(key, "--tcp-port") == 0 && val) {
        s_tcp_port = atoi(val);
        printf("[Debug] TCP port set to %d\n", s_tcp_port);
        return 1;
    }
    if (strcmp(key, "--verify") == 0) {
        g_run_mode = RUN_MODE_VERIFY;
        printf("[Verify] Dual-execution verify mode enabled\n");
        return 1;
    }
    if (strcmp(key, "--emulated") == 0) {
        g_run_mode = RUN_MODE_EMULATED;
        printf("[Verify] Nestopia emulated mode enabled\n");
        return 1;
    }
    (void)val;
    return 0;
}

const char *game_arg_usage(void) {
    return "  --tcp-port PORT     TCP debug server port (default 4370)\n"
           "  --verify            Enable dual-execution verify mode (Nestopia oracle)\n"
           "  --emulated          Run purely via Nestopia emulator (no recompiled code)\n";
}

void game_run_nmi(void) {
    verify_mode_run_nmi();
}

void game_run_main(void) {
    if (g_run_mode == RUN_MODE_EMULATED) {
#ifdef ENABLE_NESTOPIA_ORACLE
        /* Nestopia drives the entire execution -- its own CPU, PPU, APU. */
        printf("[Emulated] Nestopia driving main loop\n");

        static uint32_t nestopia_argb[256 * 240];  /* ARGB framebuffer */

        extern void runner_present_framebuf(const uint32_t *argb_buf);

        for (;;) {
            /* Poll SDL events */
            {
                SDL_Event ev;
                while (SDL_PollEvent(&ev)) {
                    if (ev.type == SDL_QUIT) exit(0);
                    if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) exit(0);
                    if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_F5)
                        g_turbo ^= 1;
                }

                /* Read keyboard for controller input */
                const uint8_t *keys = SDL_GetKeyboardState(NULL);
                uint8_t btn = 0;
                if (keys[SDL_SCANCODE_Z])      btn |= 0x80;
                if (keys[SDL_SCANCODE_X])      btn |= 0x40;
                if (keys[SDL_SCANCODE_TAB])    btn |= 0x20;
                if (keys[SDL_SCANCODE_RETURN]) btn |= 0x10;
                if (keys[SDL_SCANCODE_UP])     btn |= 0x08;
                if (keys[SDL_SCANCODE_DOWN])   btn |= 0x04;
                if (keys[SDL_SCANCODE_LEFT])   btn |= 0x02;
                if (keys[SDL_SCANCODE_RIGHT])  btn |= 0x01;
                g_controller1_buttons = btn;
            }

            /* Debug server */
            debug_server_poll();
            debug_server_wait_if_paused();

            /* Run one Nestopia frame */
            nestopia_bridge_run_frame(g_controller1_buttons);

            /* Get Nestopia's rendered framebuffer */
            nestopia_bridge_get_framebuf_argb(nestopia_argb);

            /* Present Nestopia's frame directly to SDL window */
            runner_present_framebuf(nestopia_argb);

            /* Also extract RAM state for debug server queries */
            nestopia_bridge_get_ram(g_ram);
            nestopia_bridge_get_sram(g_sram);
            g_frame_count++;

            /* Record frame for debug server */
            debug_server_record_frame();
            debug_server_check_watchpoints();

            /* Frame pacing: ~60fps */
            if (!g_turbo) SDL_Delay(16);
        }
#else
        fprintf(stderr, "[Error] Nestopia not compiled in, falling back to native\n");
        func_RESET();
#endif
    } else {
        /* Native or verify mode: func_RESET() drives the main loop,
         * NMI fires via nes_vblank_callback -> game_run_nmi(). */
        func_RESET();
    }
}

/*
 * SRAM code dispatch override.
 *
 * During init, Zelda copies ROM bank 1 $A500 -> SRAM $6C90 (~4720 bytes).
 * The game then calls subroutines in SRAM for save file management,
 * because the routines need to bank-switch the ROM window while running.
 *
 * In our static recompilation, bank switching is just a pointer change,
 * so we redirect SRAM calls to the original ROM source in bank 1.
 */
int game_dispatch_override(uint16_t addr) {
    if (addr >= 0x6C90 && addr < 0x8000) {
        int save_bank = g_current_bank;
        uint16_t rom_addr = 0xA500 + (addr - 0x6C90);
        g_current_bank = 1;
        call_by_address(rom_addr);
        g_current_bank = save_bank;
        return 1;
    }
    return 0;
}

uint8_t game_ram_read_hook(uint16_t pc, uint16_t addr, uint8_t val) {
    (void)pc; (void)addr; return val;
}
