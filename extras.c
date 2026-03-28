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
#include "recomp_stack.h"
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

/* ---- SRAM persistence ---- */
static char s_sram_path[512] = "";
static uint8_t s_sram_snapshot[0x2000]; /* last-saved snapshot for dirty check */
static int s_sram_dirty = 0;

static void sram_load(void) {
    FILE *f = fopen(s_sram_path, "rb");
    if (f) {
        size_t n = fread(g_sram, 1, 0x2000, f);
        fclose(f);
        memcpy(s_sram_snapshot, g_sram, 0x2000);
        printf("[SRAM] Loaded %zu bytes from %s\n", n, s_sram_path);
    } else {
        printf("[SRAM] No save file found, starting fresh\n");
        memcpy(s_sram_snapshot, g_sram, 0x2000);
    }
}

static void sram_save(void) {
    if (memcmp(g_sram, s_sram_snapshot, 0x2000) == 0) return; /* not dirty */
    FILE *f = fopen(s_sram_path, "wb");
    if (f) {
        fwrite(g_sram, 1, 0x2000, f);
        fclose(f);
        memcpy(s_sram_snapshot, g_sram, 0x2000);
        printf("[SRAM] Saved to %s\n", s_sram_path);
    }
}

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

uint32_t game_get_expected_crc32(void) { return 0x3FE272FBu; }

const char *game_get_name(void) { return "The Legend of Zelda"; }

/* Write breakpoint callback: log stack when ObjTimer[1] ($0029) is written */
static void on_timer1_write(uint16_t addr, uint8_t old_val, uint8_t new_val) {
    (void)addr;
    if (old_val == new_val) return;  /* skip no-change writes */
    /* Only log interesting transitions (timer set to large values or 0→nonzero) */
    if (new_val > 10 || (old_val == 0 && new_val != 0)) {
        fprintf(stderr, "[WBP] $%04X: $%02X -> $%02X  X=%d  frame=%llu  stack:",
                addr, old_val, new_val, g_cpu.X,
                (unsigned long long)g_frame_count);
        for (int i = g_recomp_stack_top - 1; i >= 0 && i >= g_recomp_stack_top - 6; i--)
            fprintf(stderr, " %s", g_recomp_stack[i]);
        fprintf(stderr, "\n");
    }
}

void game_on_init(void) {
    /* Load battery-backed SRAM from disk */
    get_exe_relative_path("zelda.srm", s_sram_path, sizeof(s_sram_path));
    sram_load();

    s_debug_enabled = check_debug_ini();

    /* Set write breakpoint on ObjTimer[1] = $0029 */
    g_write_bp_addr = 0x0029;
    g_write_bp_match_val = 0xFF;  /* any value */
    g_write_bp_callback = on_timer1_write;

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

/* Dump entity slots using correct addresses from disassembly:
 * ObjType=$034F  ObjX=$0070  ObjY=$0084  ObjDir=$0098
 * ObjState=$00AC  ObjMetastate=$0405  ObjUninitialized=$0492 */
static void dump_entities(uint64_t frame_count, const char *label) {
    printf("[%s] frame=%llu entity slots:\n", label, (unsigned long long)frame_count);
    for (int i = 0; i < 12; i++) {
        uint8_t type  = g_ram[0x34F + i];
        uint8_t x     = g_ram[0x70  + i];
        uint8_t y     = g_ram[0x84  + i];
        uint8_t state = g_ram[0xAC  + i];
        uint8_t meta  = g_ram[0x405 + i];
        uint8_t uninit= g_ram[0x492 + i];
        if (type != 0 || meta != 0)
            printf("  slot%d type=$%02X x=$%02X y=$%02X st=$%02X meta=$%02X uninit=$%02X\n",
                   i, type, x, y, state, meta, uninit);
    }
    printf("  GameMode=$%02X Sub=$%02X\n", g_ram[0x12], g_ram[0x11]);
}

void game_on_frame(uint64_t frame_count) {
    if (s_debug_enabled) {
        debug_server_poll();
        debug_server_wait_if_paused();
        int ovr = debug_server_get_input_override();
        if (ovr >= 0)
            g_controller1_buttons = (uint8_t)ovr;
    }

    /* Persist SRAM to disk every second */
    if ((frame_count % 60) == 0) sram_save();

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

/* ---- Debug server hooks (shared debug_server in nesrecomp runner) ---- */

void game_fill_frame_record(void *record) {
    NESFrameRecord *r = (NESFrameRecord *)record;
    r->game_data[0]  = g_ram[0x12];    /* GameMode */
    r->game_data[1]  = g_ram[0xEB];    /* CurrentRoom */
    r->game_data[2]  = g_ram[0x0070];  /* Link_X (slot 0) */
    r->game_data[3]  = g_ram[0x0084];  /* Link_Y (slot 0) */
    /* Entity slot 1 diagnostics (first enemy slot) */
    r->game_data[4]  = g_ram[0x350];   /* ObjType[1]  ($34F+1) */
    r->game_data[5]  = g_ram[0x406];   /* ObjMetastate[1] ($405+1) */
    r->game_data[6]  = g_ram[0x29];    /* ObjTimer[1] ($28+1) */
    r->game_data[7]  = g_ram[0x493];   /* ObjUninitialized[1] ($492+1) */
    r->game_data[8]  = g_ram[0x71];    /* ObjX[1] ($70+1) */
    r->game_data[9]  = g_ram[0x85];    /* ObjY[1] ($84+1) */
    r->game_data[10] = g_ram[0xAD];    /* ObjState[1] ($AC+1) */
    r->game_data[11] = g_ram[0x99];    /* ObjDir[1] ($98+1) */
    /* Extra context */
    r->game_data[12] = g_ram[0x26];    /* StunCycle ($26) */
    r->game_data[13] = g_ram[0x11];    /* GameSubmode */
    r->game_data[14] = g_ram[0x0F];    /* $0F (saved uninit flag during UpdateObject) */
    r->game_data[15] = g_ram[0x0340];  /* CurObjIndex ($0340) */
}

int game_handle_debug_cmd(const char *cmd, int id, const char *json) {
    (void)json;
    if (strcmp(cmd, "echo_cmd") == 0) {
        debug_server_send_fmt("{\"id\":%d,\"echo\":\"%s\"}\n", id, cmd);
        return 1;
    }
    if (strcmp(cmd, "zelda_state") == 0) {
        debug_server_send_fmt(
            "{\"id\":%d,\"game_mode\":%d,\"room\":%d,"
            "\"link_x\":%d,\"link_y\":%d,\"hp\":%d,"
            "\"sword\":%d,\"sub_mode\":%d}\n",
            id,
            g_ram[0x12],   /* GameMode */
            g_ram[0xEB],   /* CurrentRoom */
            g_ram[0x0070], /* Link_X */
            g_ram[0x0084], /* Link_Y */
            g_ram[0x066F], /* Hearts/HP */
            g_ram[0x0657], /* Sword */
            g_ram[0x13]    /* SubMode */
        );
        return 1;
    }
    /* entity_snapshot: dump all 12 entity slots with correct disasm addresses */
    if (strcmp(cmd, "entity_snapshot") == 0) {
        char buf[2048];
        int pos = snprintf(buf, sizeof(buf),
            "{\"id\":%d,\"frame\":%llu,\"mode\":%d,\"sub\":%d,\"slots\":[",
            id, (unsigned long long)g_frame_count, g_ram[0x12], g_ram[0x11]);
        for (int i = 0; i < 12; i++) {
            if (i > 0) buf[pos++] = ',';
            pos += snprintf(buf + pos, sizeof(buf) - pos,
                "{\"i\":%d,\"type\":%d,\"x\":%d,\"y\":%d,"
                "\"dir\":%d,\"state\":%d,\"meta\":%d,"
                "\"timer\":%d,\"uninit\":%d}",
                i,
                g_ram[0x34F + i],  /* ObjType */
                g_ram[0x70  + i],  /* ObjX */
                g_ram[0x84  + i],  /* ObjY */
                g_ram[0x98  + i],  /* ObjDir */
                g_ram[0xAC  + i],  /* ObjState */
                g_ram[0x405 + i],  /* ObjMetastate */
                g_ram[0x28  + i],  /* ObjTimer */
                g_ram[0x492 + i]   /* ObjUninitialized */
            );
        }
        pos += snprintf(buf + pos, sizeof(buf) - pos, "]}\n");
        debug_server_send_line(buf);
        return 1;
    }
    /* entity_slot: detailed view of one slot with extra timing context
     * Usage: {"cmd":"entity_slot","slot":1} */
    if (strcmp(cmd, "entity_slot") == 0) {
        int slot = 1;  /* default */
        const char *sp = strstr(json, "\"slot\":");
        if (sp) slot = atoi(sp + 7);
        if (slot < 0 || slot > 11) slot = 1;
        debug_server_send_fmt(
            "{\"id\":%d,\"frame\":%llu,\"slot\":%d,"
            "\"type\":\"0x%02X\",\"x\":%d,\"y\":%d,"
            "\"dir\":%d,\"state\":\"0x%02X\",\"meta\":%d,"
            "\"timer\":%d,\"uninit\":\"0x%02X\","
            "\"stun_cycle\":%d,\"paused\":%d,\"menu_state\":%d,"
            "\"mode\":%d,\"sub\":%d}\n",
            id, (unsigned long long)g_frame_count, slot,
            g_ram[0x34F + slot],
            g_ram[0x70  + slot],
            g_ram[0x84  + slot],
            g_ram[0x98  + slot],
            g_ram[0xAC  + slot],
            g_ram[0x405 + slot],
            g_ram[0x28  + slot],
            g_ram[0x492 + slot],
            g_ram[0x26],   /* StunCycle */
            g_ram[0xE0],   /* Paused */
            g_ram[0xE1],   /* MenuState */
            g_ram[0x12],   /* GameMode */
            g_ram[0x11]    /* GameSubmode */
        );
        return 1;
    }
    return 0;
}

/* ---- Watchdog globals (read by debug_server watchdog_status command) ---- */
int g_watchdog_triggered = 0;
uint64_t g_watchdog_frame = 0;
char g_watchdog_stack_dump[1024] = "";
