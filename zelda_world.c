/* World-view foundation for a continuous Zelda camera.
 *
 * Terrain is decoded by the original ROM's LayOutRoom and FillPlayAreaAttrs
 * in an isolated machine. This deliberately reuses secrets, collision tiles,
 * door graphics and quest data instead of inventing another map format.
 * Ground truth: aldonunez/zelda1-disassembly @50a1c869, Z_05.asm.
 * No ROM bytes, graphics or extracted maps are distributed here.
 */
#include "zelda_world.h"
#include "zelda_world_internal.h"
#include "cyc_presentation.h"
#include "cyc_core.h"
#include "cyc_mod.h"
#include "cyc_render.h"
#include "cyc_tcp.h"
#include "cyc_state.h"
#include "cyc_video.h"
#include "mod_runtime.h"
#include "mod_savestate.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { ROOM_W=256, ROOM_H=176, HUD_H=64, ROOMS=128,
       TILE_COLS=32, TILE_ROWS=22, TILES=TILE_COLS*TILE_ROWS,
       WORLD_W=16*ROOM_W, WORLD_H=8*ROOM_H };
typedef ZeldaWorldRoom WorldRoom;
ZeldaWorldRoom zelda_world_rooms[ROOMS];
#define rooms zelda_world_rooms
static uint8_t raster_index[ROOMS][ROOM_W*ROOM_H];
static uint32_t raster[ROOMS][ROOM_W*ROOM_H];
static uint8_t raster_tiles[ROOMS][TILES],raster_attrs[ROOMS][48],raster_palette[ROOMS][16];
static bool raster_valid[ROOMS];
static uint8_t patterns[4096],tile_pixels[256][64];
static bool patterns_valid;
static uint32_t *canvas;
static size_t canvas_capacity;
static int mode; /* 0 original; 1 terrain camera; 2 terrain atlas; 3 seamless */
static bool atlas_view;
static int draw_w=768, draw_h=720, fixed_w, fixed_h;
static int zoom=3;
static int cached_level=-1, cached_quest=-1;
static uint8_t cached_flags[128];
static unsigned decoded, failed;
static bool checked_rom, supported_rom;
static int camera_x, camera_y, picture_w, picture_h;
enum { TIMING_COUNT=1024 };
static double step_us[TIMING_COUNT],render_us[TIMING_COUNT];
static unsigned step_count,render_count;
static double handoff_max_us;
static uint64_t timing_cycle=UINT64_MAX;
static uint8_t frame_mask;
static bool death_view;
/* Presentation survives the ROM's multi-frame portal loader. Its RAM tables,
 * CHR banks and nametables can already belong to the destination while the
 * source is still on screen; never decode rooms against that partial state. */
static bool scene_valid,scene_local,scene_dark[ROOMS];
static unsigned scene_room,scene_level;
static int scene_link_x,scene_link_y;
static uint8_t scene_chr[8192],scene_oam[256],scene_palette[32];
static uint32_t scene_native[256*240];
static WorldRoom subroom;
static const char *presentation="none";
static int ui_bounds[4];
enum { SCENE_ROOM_RECORD=3+704+48+32,
       SCENE_STATE_SIZE=16+8192+256+32+256*240*4+704+48+ROOMS*SCENE_ROOM_RECORD };
static uint8_t pending_scene[SCENE_STATE_SIZE];
static bool pending_scene_valid;
static void restore_scene(void);
static void drawable_size(void) {
    int w,h;cyc_video_drawable_size(&w,&h);
    if(w>0&&h>0){draw_w=w;draw_h=h;}
}

static uint32_t crc32(const uint8_t *p,size_t n) {
    uint32_t crc=~0u;
    while(n--) { crc^=*p++;for(int b=0;b<8;b++)crc=(crc>>1)^((0u-(crc&1))&0xedb88320u); }
    return ~crc;
}
bool zelda_world_supported(void) {
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
bool zelda_world_live(void) {return mode==3;}
void zelda_world_reset(void) {
    memset(rooms,0,sizeof rooms);cached_level=cached_quest=-1;
    checked_rom=supported_rom=false;decoded=failed=0;
    patterns_valid=false;memset(raster_valid,0,sizeof raster_valid);
    scene_valid=pending_scene_valid=false;
    zelda_world_rooms_reset();
}
void zelda_world_state_loaded(void) {
    unsigned slot=cyc_mod_peek(0x16);
    cached_level=cyc_mod_peek(0x10);
    cached_quest=cyc_mod_peek((uint16_t)(0x62d+(slot<3?slot:0)));
    memset(cached_flags,0,sizeof cached_flags);
    patterns_valid=false;memset(raster_valid,0,sizeof raster_valid);
    scene_valid=false;
    if(pending_scene_valid){restore_scene();cached_level=(int)scene_level;}
    zelda_world_rooms_loaded();
}
void zelda_world_frame_end(void) {
    uint64_t cycle=cyc_cycle_count();if(cycle==timing_cycle)return;timing_cycle=cycle;
    uint64_t before=SDL_GetPerformanceCounter(),h=zelda_world_handoffs();
    zelda_world_rooms_step();
    double us=(SDL_GetPerformanceCounter()-before)*1e6/SDL_GetPerformanceFrequency();
    step_us[step_count++%TIMING_COUNT]=us;
    if(zelda_world_handoffs()!=h&&us>handoff_max_us)handoff_max_us=us;
}
void zelda_world_sim_bounds(int *l,int *t,int *r,int *b) {
    drawable_size();
    if(atlas_view){*l=0;*t=0;*r=15;*b=7;return;}
    unsigned room=cyc_mod_peek(0xeb);
    int w=fixed_w?fixed_w:draw_w/zoom,h=fixed_h?fixed_h:draw_h/zoom;
    int x=(int)(room&15)*256+cyc_mod_peek(0x70),y=(int)(room>>4)*176+cyc_mod_peek(0x84)-64;
    *l=(x-w/2)/256-1;*r=(x+w/2)/256+1;
    *t=(y-(h-64)/2)/176-1;*b=(y+(h-64)/2)/176+1;
}
static bool call(uint16_t pc,uint8_t a) {
    CycModRegs regs={0};regs.a=a;regs.s=0xfd;regs.p=0x24;
    return cyc_mod_call(pc,&regs);
}
static bool decode_to(unsigned id,WorldRoom *room) {
    /* SwitchBank is fixed-bank code. Both subsequent routines live in bank 5.
     * Isolation restores mapper state, stack, RAM, PPU, APU and native views. */
    if(!cyc_mod_isolate_begin())return false;
    bool ok=call(0xffac,5) && cyc_mod_poke(0xeb,(uint8_t)id) &&
            cyc_mod_poke(0xee,0) && call(0xa8c4,0) && call(0x83d7,(uint8_t)id);
    if(ok) {
        for(int i=0;i<TILES;i++)room->tiles[i]=cyc_mod_peek((uint16_t)(0x6530+i));
        for(int i=0;i<48;i++)room->attrs[i]=cyc_mod_peek((uint16_t)(0x530+i));
        if(!room->simulation_ready) {
            if(cyc_mod_peek(0x6b7e)==0x3f && cyc_mod_peek(0x6b7f)==0)
                for(int i=0;i<32;i++)room->palette[i]=cyc_mod_peek((uint16_t)(0x6b81+i));
            else memcpy(room->palette,cyc_ppu_palette(),32);
        }
        room->valid=true;decoded++;
    } else {room->valid=false;failed++;}
    cyc_mod_isolate_end();return ok;
}
bool zelda_world_decode_room(unsigned id) {return id<128&&decode_to(id,&rooms[id]);}
static unsigned door(unsigned id,unsigned direction) {
    unsigned a=cyc_mod_peek((uint16_t)(0x687e + id)),b=cyc_mod_peek((uint16_t)(0x68fe + id));
    if(direction==1)return (b>>2)&7;if(direction==2)return (b>>5)&7;
    if(direction==4)return (a>>2)&7;return (a>>5)&7;
}
static void ownership(unsigned level,unsigned current) {
    for(int i=0;i<128;i++)rooms[i].owned=level==0;
    if(!level)return;
    unsigned queue[128],n=0,at=0;
    unsigned start=cyc_mod_peek(0x6bad)&127;
    rooms[start].owned=true;queue[n++]=start;
    if(current!=start){rooms[current].owned=true;queue[n++]=current;}
    while(at<n) {
        unsigned id=queue[at++],x=id&15,y=id>>4;
        const int offsets[]={1,-1,16,-16};const unsigned dirs[]={1,2,4,8},reverse[]={2,1,8,4};
        for(int k=0;k<4;k++) {
            if((k==0&&x==15)||(k==1&&!x)||(k==2&&y==7)||(k==3&&!y))continue;
            unsigned next=(unsigned)((int)id+offsets[k]);
            if(rooms[next].owned||door(id,dirs[k])==1||door(next,reverse[k])==1)continue;
            /* Cellars are portal spaces, not geographic dungeon neighbors. */
            if((cyc_mod_peek((uint16_t)(0x69fe + next))&0x3e)==0x3e)continue;
            rooms[next].owned=true;queue[n++]=next;
        }
    }
}
void zelda_world_refresh(void) {
    unsigned slot=cyc_mod_peek(0x16);
    unsigned level=cyc_mod_peek(0x10), quest=cyc_mod_peek((uint16_t)(0x62d+(slot<3?slot:0)));
    unsigned room=cyc_mod_peek(0xeb);
    uint16_t flags=cyc_mod_peek(0x6baf)|(uint16_t)(cyc_mod_peek(0x6bb0)<<8);
    if(cached_level!=(int)level || cached_quest!=(int)quest) {
        memset(rooms,0,sizeof rooms);memset(cached_flags,0,sizeof cached_flags);
        cached_level=(int)level;cached_quest=(int)quest;
    }
    ownership(level,room);
    for(unsigned id=0;id<ROOMS;id++) {
        if(!rooms[id].owned)continue;
        uint8_t f=cyc_mod_peek((uint16_t)(flags+id));
        if(!rooms[id].valid || cached_flags[id]!=f) {
            /* A simulated room owns dynamic terrain. Refresh from the decoder
             * only before it has a persistent room instance. */
            if(rooms[id].simulation_ready) {
                if(level && id!=room)zelda_world_sync_doors(id,0);
                cached_flags[id]=f;
            } else if(zelda_world_decode_room(id))cached_flags[id]=f;
        }
    }
    /* Keep live changes such as burned trees, bombs, ladders and pushed blocks.
     * Tile storage is column-major, exactly as LayoutRoomOW writes it. */
    for(int i=0;i<TILES;i++)rooms[room].tiles[i]=cyc_mod_peek((uint16_t)(0x6530+i));
    for(int i=0;i<48;i++)rooms[room].attrs[i]=cyc_mod_peek((uint16_t)(0x530+i));
    rooms[room].valid=true;
}
static int clamp(int x,int lo,int hi) {return x<lo?lo:x>hi?hi:x;}
static unsigned terrain_index(int wx,int wy) {
    if(wx<0 || wx>=WORLD_W || wy<0 || wy>=WORLD_H)return 0;
    const WorldRoom *r=&rooms[(wy/ROOM_H)*16+wx/ROOM_W];
    if(!r->valid||!r->owned)return 0;
    int x=wx%ROOM_W,y=wy%ROOM_H,tx=x/8,ty=y/8;
    unsigned tile=r->tiles[tx*TILE_ROWS+ty];
    uint8_t attr=r->attrs[(ty/4)*8+tx/4];
    unsigned palette=(attr>>(((ty&2)?4:0)+((tx&2)?2:0)))&3;
    unsigned pattern=0x1000+tile*16+(y&7);
    unsigned shift=7-(x&7);
    unsigned color=((scene_chr[pattern]>>shift)&1)|
                   (((scene_chr[pattern+8]>>shift)&1)<<1);
    return color?palette*4+color:0;
}
static void prepare_rasters(int w,int h) {
    const uint8_t *now=scene_chr+4096;
    if(!patterns_valid||memcmp(now,patterns,sizeof patterns)) {
        memcpy(patterns,now,sizeof patterns);patterns_valid=true;memset(raster_valid,0,sizeof raster_valid);
        for(unsigned tile=0;tile<256;tile++)for(unsigned y=0;y<8;y++)for(unsigned x=0;x<8;x++)
            tile_pixels[tile][y*8+x]=(uint8_t)(((patterns[tile*16+y]>>(7-x))&1)|(((patterns[tile*16+y+8]>>(7-x))&1)<<1));
    }
    for(unsigned id=0;id<128;id++) {
        WorldRoom *r=&rooms[id];if(!r->valid||!r->owned)continue;
        int rx=(int)(id&15)*256,ry=(int)(id>>4)*176;
        if(rx+256<=camera_x||rx>=camera_x+w||ry+176<=camera_y||ry>=camera_y+h-HUD_H)continue;
        uint8_t colors[16];memcpy(colors,death_view?scene_palette:r->palette,16);
        if(death_view&&cyc_mod_peek(0x11)&&cyc_mod_peek(0x13)>=6)
            for(unsigned i=1;i<16;i++)colors[i]=scene_palette[12+(i&3)];
        if(scene_dark[id] && (!r->simulation_ready||!r->ram[0x51f]))memset(colors,0x0f,sizeof colors);
        if(frame_mask&1)for(unsigned i=0;i<16;i++)colors[i]&=0x30;
        bool changed=!raster_valid[id]||memcmp(raster_tiles[id],r->tiles,TILES)||memcmp(raster_attrs[id],r->attrs,48);
        if(changed) {
            memcpy(raster_tiles[id],r->tiles,TILES);memcpy(raster_attrs[id],r->attrs,48);
            for(unsigned ty=0;ty<22;ty++)for(unsigned tx=0;tx<32;tx++) {
                uint8_t attr=r->attrs[(ty/4)*8+tx/4];unsigned pal=(attr>>(((ty&2)?4:0)+((tx&2)?2:0)))&3;
                const uint8_t *pattern=tile_pixels[r->tiles[tx*22+ty]];
                for(unsigned y=0;y<8;y++)for(unsigned x=0;x<8;x++) {
                    unsigned v=pattern[y*8+x];raster_index[id][(ty*8+y)*256+tx*8+x]=(uint8_t)(v?pal*4+v:0);
                }
            }
        }
        if(changed||memcmp(colors,raster_palette[id],16)) {
            memcpy(raster_palette[id],colors,16);
            uint32_t p[16];for(unsigned i=0;i<16;i++)p[i]=g_nes_palette[colors[i]&63];
            for(unsigned i=0;i<256*176;i++)raster[id][i]=p[raster_index[id][i]];
        }
        raster_valid[id]=true;
    }
}
static void draw_actors(unsigned id,int w,int h) {
    const WorldRoom *r=&rooms[id];bool active=id==scene_room;
    if(!r->simulation_ready&&!active)return;
    int ox=(int)(id&15)*256-camera_x,oy=HUD_H+(int)(id>>4)*176-camera_y-HUD_H;
    if(ox+272<=0||ox>=w+16||oy+256<=HUD_H||oy>=h+16)return;
    for(int slot=63;slot>=18;slot--) {
        if(slot>=20&&slot<=23)continue; /* HUD icons already drawn once. */
        if(!active&&(slot==18||slot==19))continue;
        const uint8_t *o=(active?scene_oam:r->ram+0x200)+slot*4;
        if(o[0]>=0xf8)continue;
        if(o[0]==31&&slot!=18&&slot!=19)continue; /* Allocated HUD item sprites. */
        int sx=o[3],sy=o[0]+1;
        if(active&&(slot==18||slot==19)) {
            int lx=scene_link_x;sx=lx+((sx-lx+384)%256-128);
        }
        unsigned tile=o[1],attr=o[2],base=(tile&1)*0x1000+(tile&0xfe)*16;
        for(int y=0;y<16;y++)for(int x=0;x<8;x++) {
            int dx=ox+sx+x,dy=oy+sy+y;
            if(dx<0||dx>=w||dy<64||dy>=h)continue;
            unsigned py=(attr&0x80)?15-(unsigned)y:(unsigned)y,px=(attr&0x40)?(unsigned)x:7-(unsigned)x;
            unsigned a=base+(py/8)*16+(py&7);
            unsigned v=((scene_chr[a]>>px)&1)|(((scene_chr[a+8]>>px)&1)<<1);
            if(!v)continue;
            if((attr&0x20)&&(terrain_index(camera_x+dx,camera_y+dy-HUD_H)&3))continue;
            unsigned color=(active?scene_palette:r->palette)[16+(attr&3)*4+v]&63;
            if(frame_mask&1)color&=0x30;
            canvas[(size_t)dy*w+dx]=g_nes_palette[color];
        }
    }
}
static void capture_scene(bool local,const uint32_t *native) {
    bool same=scene_valid&&scene_local==local&&scene_level==cyc_mod_peek(0x10)&&scene_room==cyc_mod_peek(0xeb);
    scene_valid=true;scene_local=local;scene_level=cyc_mod_peek(0x10);scene_room=cyc_mod_peek(0xeb);
    scene_link_x=cyc_mod_peek(0x70);scene_link_y=cyc_mod_peek(0x84);
    for(unsigned i=0;i<8192;i++)scene_chr[i]=cyc_render_chr((uint16_t)i);
    memcpy(scene_palette,cyc_ppu_palette(),32);
    /* Inventory owns the native nametable/OAM while the world is frozen. */
    if(!same||!cyc_mod_peek(0xe1)) {
        memcpy(scene_native,native,sizeof scene_native);
        memcpy(scene_oam,cyc_cpu_ram()+0x200,sizeof scene_oam);
    }
    if(local) {
        for(unsigned i=0;i<TILES;i++)subroom.tiles[i]=cyc_mod_peek((uint16_t)(0x6530+i));
        for(unsigned i=0;i<48;i++)subroom.attrs[i]=cyc_mod_peek((uint16_t)(0x530+i));
    } else for(unsigned id=0;id<ROOMS;id++)
        scene_dark[id]=scene_level&&(cyc_mod_peek((uint16_t)(0x6a7e + id))&0x80)!=0;
}
static void draw_hud(const uint32_t *native,int w,int h) {
    int ox=(w-256)/2;
    for(int y=0;y<HUD_H&&y<h;y++) {
        for(int x=0;x<w;x++)canvas[(size_t)y*w+x]=native[y*256];
        for(int x=0;x<256;x++)if(ox+x>=0&&ox+x<w)canvas[(size_t)y*w+ox+x]=native[y*256+x];
    }
}
static void draw_subroom(int w,int h) {
    /* Caves/cellars are finite portal spaces. Continue their solid boundary
     * tiles past the old screen edges, with no black rectangle around the
     * room. Native playfield pixels preserve dialog, NPCs, items and fades. */
    for(int y=HUD_H;y<h;y++)for(int x=0;x<w;x++) {
        int lx=x+camera_x,ly=y-HUD_H+camera_y;
        if(lx>=0&&lx<256&&ly>=0&&ly<176) {
            canvas[(size_t)y*w+x]=scene_native[(ly+64)*256+lx];continue;
        }
        int sx=lx<0?(lx&15):lx>=256?240+(lx&15):lx;
        int sy=ly<0?(ly&15):ly>=176?160+(ly&15):ly;
        unsigned tx=(unsigned)sx/8,ty=(unsigned)sy/8;
        unsigned tile=subroom.tiles[tx*22+ty],attr=subroom.attrs[(ty/4)*8+tx/4];
        unsigned pal=(attr>>(((ty&2)?4:0)+((tx&2)?2:0)))&3;
        unsigned a=0x1000+tile*16+(sy&7),bit=7-(sx&7);
        unsigned v=((scene_chr[a]>>bit)&1)|(((scene_chr[a+8]>>bit)&1)<<1);
        unsigned color=scene_palette[v?pal*4+v:0]&63;if(frame_mask&1)color&=0x30;
        canvas[(size_t)y*w+x]=g_nes_palette[color];
    }
}
static void text(int w,int h,int x,int y,const char *s,int scale,uint32_t color) {
    for(;*s;s++,x+=8*scale) {
        unsigned tile=*s>='A'&&*s<='Z'?(unsigned)(*s-'A')+10:36;
        for(unsigned py=0;py<8;py++) {
            unsigned a=0x1000+tile*16+py;
            unsigned bits=cyc_render_chr((uint16_t)a)|cyc_render_chr((uint16_t)(a+8));
            for(unsigned px=0;px<8;px++)if(bits&(0x80>>px))
                for(int yy=0;yy<scale;yy++)for(int xx=0;xx<scale;xx++) {
                    int dx=x+(int)px*scale+xx,dy=y+(int)py*scale+yy;
                    if(dx>=0&&dx<w&&dy>=0&&dy<h)canvas[(size_t)dy*w+dx]=color;
                }
        }
    }
}
static void game_over_page(int w,int h,bool choices) {
    uint32_t black=g_nes_palette[0x0f],white=g_nes_palette[0x30],red=g_nes_palette[0x16];
    for(size_t i=0;i<(size_t)w*h;i++)canvas[i]=black;
    int scale=clamp(h/240,1,3),margin=clamp(w/12,8,w/4);
    int gap=clamp(h/10,8*scale+8,40*scale),top=h/2-gap/2;
    int title_y=choices?h/4:(h-8*scale)/2;
    text(w,h,(w-9*8*scale)/2,title_y,"GAME OVER",scale,white);
    ui_bounds[0]=margin;ui_bounds[1]=choices?top-gap/4:title_y-8;
    ui_bounds[2]=w-2*margin;ui_bounds[3]=choices?gap*3:8*scale+16;
    /* Rules and the selected row occupy the window, rather than a native
     * 256-pixel page. Input, selection, flash timing and routing stay native. */
    int rule_y=title_y+12*scale;
    if(rule_y<h)for(int x=margin;x<w-margin;x++)canvas[(size_t)rule_y*w+x]=red;
    if(!choices)return;
    static const char *labels[]={"CONTINUE","SAVE","RETRY"};
    unsigned sub=cyc_mod_peek(0x13),selected=sub&3;if(selected>2)selected=0;
    for(unsigned row=0;row<3;row++) {
        int y=top+(int)row*gap;
        bool flash=(sub&0x80)&&(cyc_mod_peek(0x29)&4);
        if(row==selected)for(int yy=y-4;yy<y+8*scale+4&&yy<h;yy++)if(yy>=0)
            for(int x=margin;x<w-margin;x++)canvas[(size_t)yy*w+x]=g_nes_palette[0x01];
        int x=(w-(int)strlen(labels[row])*8*scale)/2;
        text(w,h,x,y,labels[row],scale,row==selected&&flash?red:white);
    }
}
static void system_page(const uint32_t *native,int w,int h) {
    /* Title/profile/save/ending artwork is UI, not a world room. Keep those
     * native screens usable after Retry/Save, on the same adaptive canvas. */
    for(size_t i=0;i<(size_t)w*h;i++)canvas[i]=native[0];
    double scale=(double)w/256;if(scale>(double)h/240)scale=(double)h/240;
    int pw=(int)(256*scale),ph=(int)(240*scale),ox=(w-pw)/2,oy=(h-ph)/2;
    for(int y=0;y<ph;y++)for(int x=0;x<pw;x++)
        canvas[(size_t)(oy+y)*w+ox+x]=native[(y*240/ph)*256+x*256/pw];
}
/* The retained source is architectural presentation state only while a
 * portal loader has overwritten its guest data. Ordinary gameplay states
 * rebuild it from the machine and room instances, avoiding a per-frame blob. */
static int scene_save(uint8_t *out,int cap) {
    unsigned gm=cyc_mod_peek(0x12),sub=cyc_mod_peek(0x13);bool updating=cyc_mod_peek(0x11)!=0;
    if(!mode||!scene_valid||gm<2||gm==5||gm==8||gm==17||gm>=18||
       (gm>=13&&gm<=15)||
       (gm==3&&updating)||(gm==4&&(updating||sub>0))||
       ((gm==11||gm==12)&&(updating||sub>=8))||(gm==9&&(updating||sub>=9)))return 0;
    if(cap<SCENE_STATE_SIZE)return -1;
    memset(out,0,16);memcpy(out,"ZWP1",4);out[4]=(uint8_t)scene_local;
    out[5]=(uint8_t)scene_level;out[6]=(uint8_t)scene_room;
    out[7]=(uint8_t)scene_link_x;out[8]=(uint8_t)scene_link_y;
    uint8_t *p=out+16;
    memcpy(p,scene_chr,8192);p+=8192;memcpy(p,scene_oam,256);p+=256;
    memcpy(p,scene_palette,32);p+=32;
    for(unsigned i=0;i<256*240;i++)for(unsigned b=0;b<4;b++)*p++=(uint8_t)(scene_native[i]>>(b*8));
    memcpy(p,subroom.tiles,704);p+=704;memcpy(p,subroom.attrs,48);p+=48;
    for(unsigned id=0;id<ROOMS;id++) {
        const WorldRoom *r=&rooms[id];*p++=(uint8_t)r->valid;*p++=(uint8_t)r->owned;*p++=(uint8_t)scene_dark[id];
        memcpy(p,r->tiles,704);p+=704;memcpy(p,r->attrs,48);p+=48;memcpy(p,r->palette,32);p+=32;
    }
    return SCENE_STATE_SIZE;
}
static int scene_validate(const uint8_t *in,int len) {
    if(!len)return 1;
    if(!in||len!=SCENE_STATE_SIZE||memcmp(in,"ZWP1",4)||in[4]>1||in[5]>9||in[6]>=ROOMS)return 0;
    const uint8_t *p=in+SCENE_STATE_SIZE-ROOMS*SCENE_ROOM_RECORD;
    for(unsigned id=0;id<ROOMS;id++,p+=SCENE_ROOM_RECORD)if(p[0]>1||p[1]>1||p[2]>1)return 0;
    return 1;
}
static int scene_load(const uint8_t *in,int len) {
    if(!scene_validate(in,len))return 0;
    pending_scene_valid=len!=0;if(len)memcpy(pending_scene,in,(size_t)len);return 1;
}
static void restore_scene(void) {
    const uint8_t *in=pending_scene,*p=in+16;
    scene_valid=true;scene_local=in[4]!=0;scene_level=in[5];scene_room=in[6];scene_link_x=in[7];scene_link_y=in[8];
    memcpy(scene_chr,p,8192);p+=8192;memcpy(scene_oam,p,256);p+=256;memcpy(scene_palette,p,32);p+=32;
    for(unsigned i=0;i<256*240;i++){scene_native[i]=0;for(unsigned b=0;b<4;b++)scene_native[i]|=(uint32_t)*p++<<(b*8);}
    memcpy(subroom.tiles,p,704);p+=704;memcpy(subroom.attrs,p,48);p+=48;
    /* All mod records have now loaded; preserve the room hook's actors. */
    for(unsigned id=0;id<ROOMS;id++) {
        WorldRoom *r=&rooms[id];r->valid=*p++!=0;r->owned=*p++!=0;scene_dark[id]=*p++!=0;
        memcpy(r->tiles,p,704);p+=704;memcpy(r->attrs,p,48);p+=48;memcpy(r->palette,p,32);p+=32;
    }
    pending_scene_valid=false;
}
const uint32_t *zelda_world_present(int *width,int *height) {
    unsigned game_mode=cyc_mod_peek(0x12);
    if(!mode || !zelda_world_supported() ||
       (game_mode!=5&&game_mode!=0x11&&cached_level<0))return NULL;
    uint64_t started=SDL_GetPerformanceCounter();
    frame_mask=cyc_render_line_mask(64);death_view=game_mode==0x11;
    drawable_size();
    int w=fixed_w?fixed_w:clamp(draw_w/zoom,128,4096);
    int h=fixed_h?fixed_h:clamp(draw_h/zoom,128,2048);
    if(mode==2||atlas_view){w=WORLD_W;h=WORLD_H+HUD_H;}
    size_t count=(size_t)w*h;
    if(count>canvas_capacity) {
        uint32_t *next=realloc(canvas,count*sizeof *canvas);
        if(!next)return NULL;
        canvas=next;canvas_capacity=count;
    }
    const uint32_t *native=cyc_frame_argb();
    memset(ui_bounds,0,sizeof ui_bounds);
    if(game_mode==8||(death_view&&cyc_mod_peek(0x11)&&cyc_mod_peek(0x13)>=12)) {
        presentation=game_mode==8?"continue":"game-over";
        game_over_page(w,h,game_mode==8);goto complete;
    }
    if(game_mode<2||(game_mode>=13&&game_mode<=15)||game_mode==19) {
        presentation="system-ui";system_page(native,w,h);goto complete;
    }
    unsigned sub=cyc_mod_peek(0x13);bool updating=cyc_mod_peek(0x11)!=0;
    bool local_ready=((game_mode==11||game_mode==12)&&(updating||sub>=8))||
                     (game_mode==9&&(updating||sub>=9));
    bool world_ready=game_mode==5||death_view||(game_mode==3&&updating)||
                     (game_mode==4&&(updating||sub>0));
    if(world_ready) {
        zelda_world_refresh();capture_scene(false,native);presentation="world";
    } else if(local_ready) {
        capture_scene(true,native);presentation="subroom";
    } else if(scene_valid&&(game_mode==6||game_mode==0x10||game_mode==0x12)) {
        capture_scene(scene_local,native);presentation=scene_local?"subroom-walk":"world-walk";
    } else presentation="portal-retained";
    if(!scene_valid)return NULL;
    unsigned room=scene_room;
    int room_x=(room&15)*ROOM_W,room_y=(room>>4)*ROOM_H;
    camera_x=(mode==2||atlas_view)?0:room_x+scene_link_x+8-w/2;
    camera_y=(mode==2||atlas_view)?0:room_y+scene_link_y-HUD_H+8-(h-HUD_H)/2;
    if(scene_local) {
        camera_x=128-w/2;camera_y=(176-(h-HUD_H))/2;
        draw_subroom(w,h);draw_hud(local_ready?native:scene_native,w,h);
        goto inventory;
    }
    /* Clamp at world edges, never at individual screen edges. */
    camera_x=clamp(camera_x,w>WORLD_W?(WORLD_W-w)/2:0,w>WORLD_W?(WORLD_W-w)/2:WORLD_W-w);
    camera_y=clamp(camera_y,h-HUD_H>WORLD_H?(WORLD_H-(h-HUD_H))/2:0,
                   h-HUD_H>WORLD_H?(WORLD_H-(h-HUD_H))/2:WORLD_H-(h-HUD_H));
    prepare_rasters(w,h);
    unsigned background=scene_palette[0]&63;if(frame_mask&1)background&=0x30;
    for(size_t i=0;i<count;i++)canvas[i]=g_nes_palette[background];
    for(int y=HUD_H;y<h;y++) {
        int wy=camera_y+y-HUD_H;if(wy<0||wy>=WORLD_H)continue;
        for(int x=0;x<w;) {
            int wx=camera_x+x;if(wx<0){x++;continue;}if(wx>=WORLD_W)break;
            unsigned id=(unsigned)(wy/176)*16+(unsigned)(wx/256);
            int run=256-wx%256;if(run>w-x)run=w-x;
            if(rooms[id].owned&&rooms[id].valid)memcpy(canvas+(size_t)y*w+x,raster[id]+(wy%176)*256+wx%256,(size_t)run*sizeof *canvas);
            x+=run;
        }
    }
    draw_hud(world_ready||game_mode==6||game_mode==0x10?native:scene_native,w,h);
    if(mode==3) {
        if(!death_view)for(unsigned id=0;id<128;id++)if(id!=room&&rooms[id].owned)draw_actors(id,w,h);
        draw_actors(room,w,h);
    } else {
        for(int y=0;y<ROOM_H;y++)for(int x=0;x<ROOM_W;x++) {
            int dx=room_x+x-camera_x,dy=HUD_H+room_y+y-camera_y;
            if(dx>=0&&dx<w&&dy>=HUD_H&&dy<h)canvas[(size_t)dy*w+dx]=scene_native[(y+HUD_H)*ROOM_W+x];
        }
    }
inventory:
    if(game_mode==5||local_ready) {
        /* The native inventory pushes its playfield down from the HUD. Use
         * that frame's actual split scroll, but shift our current world view,
         * never the stale native nametable underneath the inventory. */
        int panel=clamp((64-cyc_render_line_scroll_y(64)+480)%480,0,175);
        if(panel) {
            int hud_x=(w-ROOM_W)/2;
            if(panel>h-HUD_H)panel=h-HUD_H;
            memmove(canvas+(size_t)(HUD_H+panel)*w,canvas+(size_t)HUD_H*w,
                (size_t)(h-HUD_H-panel)*w*sizeof *canvas);
            for(int y=HUD_H;y<HUD_H+panel;y++) {
                for(int x=0;x<w;x++)canvas[(size_t)y*w+x]=native[y*ROOM_W];
                for(int x=0;x<ROOM_W;x++)if(hud_x+x>=0&&hud_x+x<w)
                    canvas[(size_t)y*w+hud_x+x]=native[y*ROOM_W+x];
            }
        }
    }
complete:
    picture_w=w;picture_h=h;*width=w;*height=h;
    render_us[render_count++%TIMING_COUNT]=(SDL_GetPerformanceCounter()-started)*1e6/SDL_GetPerformanceFrequency();
    return canvas;
}
bool zelda_world_option(const char *name,const char *value) {
    if(!strcmp(name,"--world-view")) {
        if(!strcmp(value,"off"))mode=0;
        else if(!strcmp(value,"follow"))mode=1;
        else if(!strcmp(value,"atlas"))mode=2;
        else if(!strcmp(value,"seamless"))mode=3;
        else return false;
        zelda_world_enable_hooks(mode==3);
        if(mode==3)nes_mod_set_local_only("Seamless world",1);
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
    if(e->type==SDL_KEYDOWN && !e->key.repeat && e->key.keysym.sym==SDLK_F4)atlas_view=!atlas_view;
}
static void world_state(int id,const char *line) {
    (void)line;char out[1024];
    unsigned simulated=0,actors=0;uint64_t ticks=0;
    for(unsigned id=0;id<128;id++)if(rooms[id].simulation_ready){simulated++;ticks+=rooms[id].ticks;for(unsigned s=1;s<=11;s++)actors+=rooms[id].ram[0x34f+s]!=0;}
    snprintf(out,sizeof out,"\"enabled\":%s,\"stage\":\"room-simulation\",\"level\":%d,\"decoded\":%u,\"failures\":%llu,\"camera\":[%d,%d],\"size\":[%d,%d],\"adjacent_actors\":%s,\"seamless_traversal\":%s,\"simulated_rooms\":%u,\"actors\":%u,\"neighbor_ticks\":%llu,\"handoffs\":%llu",
             mode?"true":"false",cached_level,decoded,(unsigned long long)(failed+zelda_world_sim_failures()),camera_x,camera_y,picture_w,picture_h,mode==3?"true":"false",mode==3?"true":"false",simulated,actors,(unsigned long long)ticks,(unsigned long long)zelda_world_handoffs());
    size_t used=strlen(out);
    snprintf(out+used,sizeof out-used,",\"menu\":%u,\"game_mode\":%u,\"submode\":%u,\"updating\":%u,\"scroll_y64\":%d,\"mask64\":%u,\"presentation\":\"%s\",\"scene_room\":%u,\"scene_level\":%u,\"ui_bounds\":[%d,%d,%d,%d]",
        cyc_mod_peek(0xe1),cyc_mod_peek(0x12),cyc_mod_peek(0x13),cyc_mod_peek(0x11),cyc_render_line_scroll_y(64),cyc_render_line_mask(64),presentation,scene_room,scene_level,ui_bounds[0],ui_bounds[1],ui_bounds[2],ui_bounds[3]);
    cyc_tcp_ok(id,out);
}
static int compare_time(const void *a,const void *b) {double x=*(const double *)a,y=*(const double *)b;return (x>y)-(x<y);}
static void world_timing(int id,const char *line) {
    bool reset=false;cyc_tcp_bool(line,"reset",&reset);
    double a[TIMING_COUNT],b[TIMING_COUNT];
    unsigned na=step_count<TIMING_COUNT?step_count:TIMING_COUNT,nb=render_count<TIMING_COUNT?render_count:TIMING_COUNT;
    memcpy(a,step_us,na*sizeof *a);memcpy(b,render_us,nb*sizeof *b);
    qsort(a,na,sizeof *a,compare_time);qsort(b,nb,sizeof *b,compare_time);
    char out[512];snprintf(out,sizeof out,"\"samples\":[%u,%u],\"simulation_us\":[%.1f,%.1f,%.1f],\"render_us\":[%.1f,%.1f,%.1f],\"handoff_simulation_max_us\":%.1f",
        na,nb,na?a[na/2]:0,na?a[na*95/100]:0,na?a[na-1]:0,nb?b[nb/2]:0,nb?b[nb*95/100]:0,nb?b[nb-1]:0,handoff_max_us);
    cyc_tcp_ok(id,out);
    if(reset){step_count=render_count=0;handoff_max_us=0;}
}
static void world_probe(int id,const char *line) {
    (void)line;
    uint8_t *before=NULL,*after=NULL;size_t nb=0,na=0;char err[256];
    bool saved=cyc_state_save(&before,&nb,err,sizeof err);
    bool decoded_ok=false, tiles_exact=false, attrs_exact=false;
    if(saved && zelda_world_supported() && cyc_mod_peek(0x12)==5) {
        unsigned room=cyc_mod_peek(0xeb);
        WorldRoom temporary={0};decoded_ok=decode_to(room,&temporary);
        if(decoded_ok) {
            tiles_exact=attrs_exact=true;
            for(int i=0;i<TILES;i++)if(temporary.tiles[i]!=cyc_mod_peek((uint16_t)(0x6530+i)))tiles_exact=false;
            for(int i=0;i<48;i++)if(temporary.attrs[i]!=cyc_mod_peek((uint16_t)(0x530+i)))attrs_exact=false;
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
    cyc_tcp_register("zelda_world_timing","World simulation/render p50, p95 and maximum microseconds",world_timing);
    cyc_tcp_register("zelda_world","Development world camera and coverage",world_state);
    cyc_tcp_register("zelda_world_probe","Check guest decoder against live room and full machine isolation",world_probe);
    zelda_world_rooms_tcp_setup();
}

static void reset_world_plugin(void) {mode=0;fixed_w=fixed_h=0;atlas_view=false;zelda_world_enable_hooks(false);}
static void activate_world_plugin(void) {
    mode=3;zoom=nes_mod_get_option_int("legend-of-zelda.enhancement.seamless-world","seamless-world","pixel-zoom",3);
    zelda_world_enable_hooks(true);nes_mod_set_local_only("Seamless world",1);
}
NES_MOD_CONSTRUCTOR(register_world_plugin) {
    if(!nes_mod_register_reset_callback(reset_world_plugin)||
       !nes_mod_register_activation_plugin("legend-of-zelda.seamless-world",activate_world_plugin)||
       !nes_mod_register_savestate_hook("zelda.world.portal-view",scene_save,scene_load)||
       !nes_mod_register_savestate_validator("zelda.world.portal-view",scene_validate))abort();
}
