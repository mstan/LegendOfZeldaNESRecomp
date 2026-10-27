/* World-view foundation for a continuous Zelda camera.
 *
 * Terrain is decoded by the original ROM's LayOutRoom and FillPlayAreaAttrs
 * in an isolated machine. This deliberately reuses secrets, collision tiles,
 * door graphics and quest data instead of inventing another map format.
 * Ground truth: aldonunez/zelda1-disassembly @50a1c869, Z_05.asm.
 * No ROM bytes, graphics or extracted maps are distributed here.
 */
#include "zelda_world.h"
#include "cyc_core.h"
#include "cyc_mod.h"
#include "cyc_render.h"
#include "cyc_tcp.h"
#include "cyc_state.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { ROOM_W=256, ROOM_H=176, HUD_H=64, ROOMS=128,
       TILE_COLS=32, TILE_ROWS=22, TILES=TILE_COLS*TILE_ROWS,
       WORLD_W=16*ROOM_W, WORLD_H=8*ROOM_H };
typedef struct { uint8_t tiles[TILES], attrs[48]; bool valid; } WorldRoom;
static WorldRoom rooms[ROOMS];
static uint32_t *canvas;
static size_t canvas_capacity;
static int mode; /* 0 original; 1 following camera; 2 full terrain atlas */
static int draw_w=768, draw_h=720, fixed_w, fixed_h;
static int zoom=3;
static int cached_level=-1, cached_quest=-1;
static uint8_t cached_flags[128];
static unsigned decoded, failed;
static bool checked_rom, supported_rom;
static int camera_x, camera_y, picture_w, picture_h;

static uint32_t crc32(const uint8_t *p,size_t n) {
    uint32_t crc=~0u;
    while(n--) { crc^=*p++;for(int b=0;b<8;b++)crc=(crc>>1)^((0u-(crc&1))&0xedb88320u); }
    return ~crc;
}
static bool supported(void) {
    if(!checked_rom) {
        size_t n=cyc_mod_copy_cart_payload(NULL,0);
        if(n==128u*1024u) {
            uint8_t *prg=malloc(n);
            if(prg) { cyc_mod_copy_cart_payload(prg,n);supported_rom=crc32(prg,n)==0x3fe272fbu;free(prg); }
        }
        checked_rom=true;
        if(!supported_rom)fprintf(stderr,"[zelda world] requires unmodified USA PRG0 ROM; original display retained\n");
    }
    return supported_rom;
}
void zelda_world_reset(void) {
    memset(rooms,0,sizeof rooms);cached_level=cached_quest=-1;
    checked_rom=supported_rom=false;decoded=failed=0;
}
static bool call(uint16_t pc,uint8_t a) {
    CycModRegs regs={0};regs.a=a;regs.s=0xfd;regs.p=0x24;
    return cyc_mod_call(pc,&regs);
}
static bool decode_room(unsigned id) {
    /* SwitchBank is fixed-bank code. Both subsequent routines live in bank 5.
     * Isolation restores mapper state, stack, RAM, PPU, APU and native views. */
    if(!cyc_mod_isolate_begin())return false;
    bool ok=call(0xffac,5) && cyc_mod_poke(0xeb,(uint8_t)id) &&
            cyc_mod_poke(0xee,0) && call(0xa8c4,0) && call(0x83d7,(uint8_t)id);
    if(ok) {
        for(int i=0;i<TILES;i++)rooms[id].tiles[i]=cyc_mod_peek((uint16_t)(0x6530+i));
        for(int i=0;i<48;i++)rooms[id].attrs[i]=cyc_mod_peek((uint16_t)(0x530+i));
        rooms[id].valid=true;decoded++;
    } else {rooms[id].valid=false;failed++;}
    cyc_mod_isolate_end();return ok;
}
static void refresh(void) {
    unsigned slot=cyc_mod_peek(0x16);
    unsigned level=cyc_mod_peek(0x10), quest=cyc_mod_peek((uint16_t)(0x62d+(slot<3?slot:0)));
    unsigned room=cyc_mod_peek(0xeb);
    uint16_t flags=cyc_mod_peek(0x6baf)|(uint16_t)(cyc_mod_peek(0x6bb0)<<8);
    if(cached_level!=(int)level || cached_quest!=(int)quest) {
        memset(rooms,0,sizeof rooms);memset(cached_flags,0,sizeof cached_flags);
        cached_level=(int)level;cached_quest=(int)quest;
    }
    /* Only the overworld's complete geographic ownership is established in
     * this first slice. Dungeon membership/portal topology is a separate gate;
     * displaying all shared UW attribute rows would invent non-existent rooms. */
    if(level)return;
    for(unsigned id=0;id<ROOMS;id++) {
        uint8_t f=cyc_mod_peek((uint16_t)(flags+id));
        if(!rooms[id].valid || cached_flags[id]!=f) {
            if(decode_room(id))cached_flags[id]=f;
        }
    }
    /* Keep live changes such as burned trees, bombs, ladders and pushed blocks.
     * Tile storage is column-major, exactly as LayoutRoomOW writes it. */
    for(int i=0;i<TILES;i++)rooms[room].tiles[i]=cyc_mod_peek((uint16_t)(0x6530+i));
    for(int i=0;i<48;i++)rooms[room].attrs[i]=cyc_mod_peek((uint16_t)(0x530+i));
    rooms[room].valid=true;
}
static int clamp(int x,int lo,int hi) {return x<lo?lo:x>hi?hi:x;}
static uint32_t terrain_pixel(int wx,int wy) {
    if(wx<0 || wx>=WORLD_W || wy<0 || wy>=WORLD_H)return 0xff000000u;
    const WorldRoom *r=&rooms[(wy/ROOM_H)*16+wx/ROOM_W];
    if(!r->valid)return 0xff000000u;
    int x=wx%ROOM_W,y=wy%ROOM_H,tx=x/8,ty=y/8;
    unsigned tile=r->tiles[tx*TILE_ROWS+ty];
    uint8_t attr=r->attrs[(ty/4)*8+tx/4];
    unsigned palette=(attr>>(((ty&2)?4:0)+((tx&2)?2:0)))&3;
    unsigned pattern=0x1000+tile*16+(y&7);
    unsigned shift=7-(x&7);
    unsigned color=((cyc_render_chr((uint16_t)pattern)>>shift)&1)|
                   (((cyc_render_chr((uint16_t)(pattern+8))>>shift)&1)<<1);
    return cyc_render_color(color?(int)(palette*4+color):0);
}
const uint32_t *zelda_world_present(int *width,int *height) {
    if(!mode || cyc_mod_peek(0x12)!=5 || cyc_mod_peek(0x10)!=0 ||
       cyc_mod_peek(0xe1)!=0 || !supported())return NULL;
    refresh();
    int w=fixed_w?fixed_w:clamp(draw_w/zoom,128,4096);
    int h=fixed_h?fixed_h:clamp(draw_h/zoom,128,2048);
    if(mode==2){w=WORLD_W;h=WORLD_H+HUD_H;}
    size_t count=(size_t)w*h;
    if(count>canvas_capacity) {
        uint32_t *next=realloc(canvas,count*sizeof *canvas);
        if(!next)return NULL;
        canvas=next;canvas_capacity=count;
    }
    unsigned room=cyc_mod_peek(0xeb);
    int room_x=(room&15)*ROOM_W,room_y=(room>>4)*ROOM_H;
    camera_x=mode==2?0:room_x+cyc_mod_peek(0x70)+8-w/2;
    camera_y=mode==2?0:room_y+cyc_mod_peek(0x84)-HUD_H+8-(h-HUD_H)/2;
    /* Clamp at world edges, never at individual screen edges. */
    camera_x=clamp(camera_x,w>WORLD_W?(WORLD_W-w)/2:0,w>WORLD_W?(WORLD_W-w)/2:WORLD_W-w);
    camera_y=clamp(camera_y,h-HUD_H>WORLD_H?(WORLD_H-(h-HUD_H))/2:0,
                   h-HUD_H>WORLD_H?(WORLD_H-(h-HUD_H))/2:WORLD_H-(h-HUD_H));
    for(int y=0;y<h;y++)for(int x=0;x<w;x++)
        canvas[(size_t)y*w+x]=y<HUD_H?0xff000000u:terrain_pixel(camera_x+x,camera_y+y-HUD_H);
    const uint32_t *native=cyc_frame_argb();
    int hud_x=(w-ROOM_W)/2;
    for(int y=0;y<HUD_H;y++)for(int x=0;x<ROOM_W;x++)if(hud_x+x>=0&&hud_x+x<w)
        canvas[(size_t)y*w+hud_x+x]=native[y*ROOM_W+x];
    /* Current room uses the actual PPU image, preserving sprites and live
     * background priority. Adjacent room actors are not implemented yet. */
    for(int y=0;y<ROOM_H;y++)for(int x=0;x<ROOM_W;x++) {
        int dx=room_x+x-camera_x,dy=HUD_H+room_y+y-camera_y;
        if(dx>=0&&dx<w&&dy>=HUD_H&&dy<h)
            canvas[(size_t)dy*w+dx]=native[(y+HUD_H)*ROOM_W+x];
    }
    picture_w=w;picture_h=h;*width=w;*height=h;return canvas;
}
bool zelda_world_option(const char *name,const char *value) {
    if(!strcmp(name,"--world-view")) {
        if(!strcmp(value,"off"))mode=0;
        else if(!strcmp(value,"follow"))mode=1;
        else if(!strcmp(value,"atlas"))mode=2;
        else return false;
        return true;
    }
    if(!strcmp(name,"--world-size")) {
        int w,h;char extra;
        if(sscanf(value,"%dx%d%c",&w,&h,&extra)!=2 || w<128 || w>4096 || h<128 || h>2048)return false;
        fixed_w=w;fixed_h=h;return true;
    }
    return false;
}
void zelda_world_event(const SDL_Event *e) {
    if(!mode)return;
    if(e->type==SDL_WINDOWEVENT&&e->window.event==SDL_WINDOWEVENT_SIZE_CHANGED) {
        draw_w=clamp(e->window.data1,1,32768);draw_h=clamp(e->window.data2,1,32768);
    }
    if(e->type==SDL_MOUSEWHEEL)zoom=clamp(zoom-e->wheel.y,1,8);
}
static void world_state(int id,const char *line) {
    (void)line;char out[768];
    snprintf(out,sizeof out,"\"enabled\":%s,\"stage\":\"terrain-camera\",\"level\":%d,\"decoded\":%u,\"failures\":%u,\"camera\":[%d,%d],\"size\":[%d,%d],\"adjacent_actors\":false,\"seamless_traversal\":false",
             mode?"true":"false",cached_level,decoded,failed,camera_x,camera_y,picture_w,picture_h);
    cyc_tcp_ok(id,out);
}
static void world_probe(int id,const char *line) {
    (void)line;
    uint8_t *before=NULL,*after=NULL;size_t nb=0,na=0;char err[256];
    bool saved=cyc_state_save(&before,&nb,err,sizeof err);
    bool decoded_ok=false, tiles_exact=false, attrs_exact=false;
    if(saved && supported() && cyc_mod_peek(0x12)==5 && cyc_mod_peek(0x10)==0) {
        unsigned room=cyc_mod_peek(0xeb);
        decoded_ok=decode_room(room);
        if(decoded_ok) {
            tiles_exact=attrs_exact=true;
            for(int i=0;i<TILES;i++)if(rooms[room].tiles[i]!=cyc_mod_peek((uint16_t)(0x6530+i)))tiles_exact=false;
            for(int i=0;i<48;i++)if(rooms[room].attrs[i]!=cyc_mod_peek((uint16_t)(0x530+i)))attrs_exact=false;
        }
    }
    bool saved_after=saved && cyc_state_save(&after,&na,err,sizeof err);
    bool unchanged=saved_after && nb==na && !memcmp(before,after,nb);
    free(before);free(after);
    char out[512];snprintf(out,sizeof out,"\"decoded\":%s,\"live_tiles_exact\":%s,\"live_attrs_exact\":%s,\"machine_state_unchanged\":%s",
        decoded_ok?"true":"false",tiles_exact?"true":"false",attrs_exact?"true":"false",unchanged?"true":"false");
    cyc_tcp_ok(id,out);
}
void zelda_world_tcp_setup(void) {
    cyc_tcp_register("zelda_world","Development world camera and coverage",world_state);
    cyc_tcp_register("zelda_world_probe","Check guest decoder against live room and full machine isolation",world_probe);
}
