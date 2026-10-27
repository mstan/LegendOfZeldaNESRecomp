/* Original Zelda actor simulation in independently owned room instances.
 *
 * The active room stays on the ordinary cycle machine. Neighbors run the
 * original UpdateMode5Play in isolated scopes, with their own actors, timers,
 * RNG, tile map and sprite list. Only explicitly room-owned memory is adopted
 * at a geographic seam. Inventory, saves, CPU stack, audio, controller state,
 * player state, global progression and the machine clock keep their owner.
 * Source: zelda1-disassembly 50a1c869, Variables.inc, Z_01/05/07.asm.
 */
#include "zelda_world_internal.h"
#include "cyc_core.h"
#include "cyc_mod.h"
#include "cyc_tcp.h"
#include "mod_function_hooks.h"
#include "mod_runtime.h"
#include "mod_savestate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t local[2048];
static bool mask_ready, simulating;
static int level=-1, quest=-1, previous_room=-1, previous_x,previous_y;
static uint64_t last_cycle=UINT64_MAX, handoffs, failures;
static const char *hook_ids[]={"zelda.world.edge","zelda.world.player",
    "zelda.world.moving-tile","zelda.world.still-tile","zelda.world.doorway"};
static const uint16_t hook_pcs[]={0xf161,0xeda0,0xedfa,0xedf4,0x917c};
static uint8_t peek(unsigned a) {return cyc_mod_peek((uint16_t)a);}
static void poke(unsigned a,unsigned v) {(void)cyc_mod_poke((uint16_t)a,(uint8_t)v);}
static bool call(uint16_t pc,uint8_t a) {
    CycModRegs r={0};r.a=a;r.s=0xfd;r.p=0x24;return cyc_mod_call(pc,&r);
}
static void range(unsigned a,unsigned b,bool value) {for(;a<=b;a++)local[a]=(uint8_t)value;}
static void make_mask(void) {
    if(mask_ready)return;
    /* Timers, chase/door state, non-player objects, local enemy/boss scratch,
     * tile-object state and room attributes. Never copy the 6502 stack. */
    local[0x27]=1;range(0x29,0x3b,true);range(0x3e,0x4b,true);local[0x4d]=1;
    range(0x53,0x55,true);local[0x59]=1;range(0x60,0x62,true);
    range(0x70,0xdf,true);range(0x33f,0x51f,true);
    local[0xee]=1;local[0x521]=1;range(0x524,0x525,true);
    range(0x52b,0x52d,true);range(0x530,0x55f,true);
    /* Slot zero is Link. All of these arrays include him; his state travels
     * with the player, rather than being replaced by a neighboring ghost. */
    const unsigned player[]={0x70,0x84,0x98,0xac,0xc0,0xd3,0x380,0x394,
        0x3a8,0x3bc,0x3d0,0x3e4,0x3f8,0x405,0x4f0};
    for(unsigned i=0;i<sizeof player/sizeof *player;i++)local[player[i]]=0;
    /* Player-wide events and asset/fade state are not room-local. */
    range(0x504,0x506,false);range(0x508,0x509,false);local[0x512]=0;
    range(0x51c,0x51d,false);
    mask_ready=true;
}
bool zelda_world_portal(unsigned room) {
    return peek(0x10)==0 && (room==0x61 || room==0x1b);
}
int zelda_world_neighbor(unsigned room,unsigned dir) {
    int x=(int)(room&15),y=(int)(room>>4);
    if(dir==1)x++;else if(dir==2)x--;else if(dir==4)y++;else if(dir==8)y--;else return -1;
    if(x<0||x>=16||y<0||y>=8)return -1;
    int next=y*16+x;
    return zelda_world_rooms[next].owned?next:-1;
}
void zelda_world_enable_hooks(bool enabled) {
    for(unsigned i=0;i<sizeof hook_ids/sizeof *hook_ids;i++)
        nes_mod_set_function_hook_enabled(hook_ids[i],enabled?1:0);
}
void zelda_world_rooms_reset(void) {
    make_mask();level=quest=previous_room=-1;last_cycle=UINT64_MAX;
    handoffs=failures=0;simulating=false;
}
void zelda_world_rooms_loaded(void) {simulating=false;}
uint64_t zelda_world_handoffs(void) {return handoffs;}
uint64_t zelda_world_sim_failures(void) {return failures;}

static void capture(unsigned id) {
    ZeldaWorldRoom *r=&zelda_world_rooms[id];
    memcpy(r->ram,cyc_cpu_ram(),sizeof r->ram);
    for(unsigned i=0;i<704;i++)r->tiles[i]=peek(0x6530+i);
    for(unsigned i=0;i<48;i++)r->attrs[i]=peek(0x530+i);
    memcpy(r->palette,cyc_ppu_palette(),32);
    /* Same palette selector as Z_07:InitMode5Play, retained for re-entry. */
    if(!peek(0x10)) {
        unsigned type=peek(0x35a),inner=peek(0x68fe + id)&1;
        r->ram[0x14]=(uint8_t)(type==0x65?0x20:type==0x66?(inner?0x20:0x22):type==0x62?(inner?0x24:0x22):0x7a);
    } else {
        const uint8_t types[]={0x3d,0x3e,0x38,0x39,0x32,0x31,0x43,0x44,0x45};
        const uint8_t rows[]={0x08,0x36,0x0a,0x0a,0x0a,0x0a,0x7c,0x7c,0x7c};
        r->ram[0x14]=6;
        for(unsigned i=0;i<sizeof types;i++)if(peek(0x350)==types[i])r->ram[0x14]=rows[i];
    }
    r->simulation_ready=r->valid=true;
}
static void load_local(unsigned id,bool sprites) {
    ZeldaWorldRoom *r=&zelda_world_rooms[id];
    for(unsigned a=0;a<2048;a++)if(local[a])poke(a,r->ram[a]);
    for(unsigned i=0;i<704;i++)poke(0x6530+i,r->tiles[i]);
    if(sprites)for(unsigned a=0x250;a<0x300;a++)poke(a,r->ram[a]);
    poke(0xeb,id);poke(0xec,id);
}
static void shadow_player(unsigned id,unsigned active,int ax,int ay) {
    int x=((int)(active&15)-(int)(id&15))*256+ax;
    int y=((int)(active>>4)-(int)(id>>4))*176+ay;
    /* Original AI uses 8-bit positions. The nearest boundary point preserves
     * its pursuit direction while the actual player is in another room. */
    if(x<0)x=0;if(x>240)x=240;if(y<64)y=64;if(y>224)y=224;
    poke(0x70,(unsigned)x);poke(0x84,(unsigned)y);
    poke(0x4f0,255);poke(0xf8,0);poke(0xfa,0);poke(0x3f8,0);
    poke(0xe0,0);poke(0xe1,0);
}
void zelda_world_sync_doors(unsigned id,unsigned entering) {
    ZeldaWorldRoom *r=&zelda_world_rooms[id];
    if(!r->simulation_ready||!peek(0x10))return;
    if(!cyc_mod_isolate_begin()){failures++;return;}
    load_local(id,false);
    poke(0xee,peek(0xee)|entering);
    bool ok=call(0xffac,5)&&call(0xb66c,0)&&call(0xa4f6,0);
    if(ok) {
        for(unsigned i=0;i<704;i++)r->tiles[i]=peek(0x6530+i);
        r->ram[0xee]=peek(0xee);
    } else failures++;
    cyc_mod_isolate_end();
}
static bool initialize(unsigned id,unsigned active,int ax,int ay) {
    if(!cyc_mod_isolate_begin())return false;
    bool ok=call(0xffac,5);
    poke(0xeb,id);poke(0xec,id);poke(0xee,0);poke(0x5a,0);
    poke(0x12,4);poke(0x11,0);poke(0x13,0);
    shadow_player(id,active,ax,ay);
    if(ok)ok=call(0xa8c4,0) && call(0x83d7,(uint8_t)id) && call(0x87c6,0);
    if(ok) {
        poke(0x12,5);poke(0x11,0);poke(0x13,0);poke(0x394,0);
        ok=call(0xea6b,0);
    }
    /* Give each room the level's palette, then its native boss/tile-object
     * row. Transfers run only on the scratch machine, with no guest clock. */
    if(ok) {
        unsigned row=peek(0x14);
        poke(0x14,0x18);ok=call(0xffac,6)&&call(0xa080,0);
        if(ok&&row){poke(0x14,row);ok=call(0xa080,0);}
    }
    if(ok) {
        for(unsigned a=0x248;a<0x300;a+=4)poke(a,0xf8);
        shadow_player(id,active,ax,ay);capture(id);
        zelda_world_rooms[id].ticks=0;
    }
    cyc_mod_isolate_end();
    if(!ok)failures++;
    return ok;
}
static void timers(const ZeldaWorldRoom *r) {
    unsigned stun=r->ram[0x26];bool long_tick=stun==0;
    poke(0x26,long_tick?9:stun-1);
    for(unsigned a=0x27;a<=0x3c;a++){unsigned v=peek(a);if(v)poke(a,v-1);}
    if(long_tick)for(unsigned a=0x3d;a<=0x4e;a++){unsigned v=peek(a);if(v)poke(a,v-1);}
    /* Exact 13-byte feedback shift from Z_07:IsrNmi/@ScrambleRandom. */
    unsigned carry=((r->ram[0x18]^r->ram[0x19])&2)!=0;
    for(unsigned a=0x18;a<=0x24;a++) {
        unsigned v=r->ram[a],next=v&1;poke(a,(v>>1)|(carry<<7));carry=next;
    }
}
static void tick(unsigned id,unsigned active,int ax,int ay) {
    ZeldaWorldRoom *r=&zelda_world_rooms[id];
    if(!r->simulation_ready && !initialize(id,active,ax,ay))return;
    if(!cyc_mod_isolate_begin()){failures++;return;}
    load_local(id,false);shadow_player(id,active,ax,ay);
    timers(r);poke(0x12,5);poke(0x11,1);poke(0x13,0);poke(0x340,11);
    for(unsigned a=0x248;a<0x300;a+=4)poke(a,0xf8);
    simulating=true;cyc_mod_allow_isolated_hooks(true);
    bool ok=call(0xec1b,0);
    simulating=false;
    if(ok) {
        uint8_t palette[32];memcpy(palette,r->palette,32);
        capture(id);memcpy(r->palette,palette,32);r->ticks++;
    } else failures++;
    cyc_mod_isolate_end();
}
static bool adopt(unsigned next,int x,int y) {
    unsigned old=peek(0xeb);
    ZeldaWorldRoom *dest=&zelda_world_rooms[next];
    if(!dest->simulation_ready && !initialize(next,old,peek(0x70),peek(0x84)))return false;
    if(peek(0x10)) {
        unsigned incoming=next==old+1?2:next+1==old?1:next==old+16?8:next+16==old?4:0;
        zelda_world_sync_doors(next,incoming);
    }
    /* SaveKillCount owns shared progress. Keep its intended memory effects,
     * never the speculative neighbor's inventory, audio or save-file bytes. */
    if(cyc_mod_isolate_begin()) {
        uint8_t flags[0x180],kills[0x90];
        bool ok=call(0xffac,5)&&call(0xb464,0);
        if(ok){for(unsigned i=0;i<sizeof flags;i++)flags[i]=peek(0x67f+i);
            for(unsigned i=0;i<sizeof kills;i++)kills[i]=peek(0x560+i);}
        cyc_mod_isolate_end();
        if(ok){for(unsigned i=0;i<sizeof flags;i++)poke(0x67f+i,flags[i]);
            for(unsigned i=0;i<sizeof kills;i++)poke(0x560+i,kills[i]);}
        else {failures++;return false;}
    } else {failures++;return false;}
    /* Kill totals are committed once; the persistent room keeps its actors. */
    zelda_world_rooms[old].ram[0x34f]=0;
    /* Sword slot 13 belongs to Link's current swing. Other projectiles stay
     * owned by the room in which they were created. */
    const unsigned sword_fields[]={0x70,0x84,0x98,0xac,0xc0,0xd3,0x380,0x394,
        0x3a8,0x3bc,0x3d0,0x3e4,0x3f8,0x405,0x485,0x492,0x49e,0x4bf,0x4f0};
    uint8_t sword[sizeof sword_fields/sizeof *sword_fields];
    for(unsigned i=0;i<sizeof sword_fields/sizeof *sword_fields;i++)sword[i]=peek(sword_fields[i]+13);
    sword[0]=(uint8_t)(sword[0]+x-(int)peek(0x70));
    sword[1]=(uint8_t)(sword[1]+y-(int)peek(0x84));
    zelda_world_rooms[old].ram[0xac+13]=0;
    load_local(next,true);poke(0x70,(unsigned)x);poke(0x84,(unsigned)y);
    for(unsigned i=0;i<sizeof sword_fields/sizeof *sword_fields;i++)poke(sword_fields[i]+13,sword[i]);
    poke(0x14,dest->ram[0x14]);
    poke(0x340,11);poke(0x53,0);poke(0x54,0);poke(0x55,0);
    /* Remember the native room history without respawning cached actors. */
    bool found=false;for(unsigned i=0;i<6;i++)if(peek(0x621+i)==next)found=true;
    if(!found){unsigned at=peek(0x620)%6;poke(0x621+at,next);poke(0x620,(at+1)%6);}
    if(peek(0x10)){unsigned addr=peek(0x6baf)|(peek(0x6bb0)<<8);poke(addr+next,peek(addr+next)|0x20);}
    handoffs++;return true;
}
void zelda_world_rooms_step(void) {
    if(!zelda_world_live() || !zelda_world_supported())return;
    uint64_t cycle=cyc_cycle_count();
    if(cycle==last_cycle)return;
    last_cycle=cycle;
    unsigned slot=peek(0x16);int q=peek(0x62d+(slot<3?slot:0)),lev=peek(0x10);
    if(lev!=level||q!=quest){level=lev;quest=q;previous_room=-1;}
    if(peek(0x12)!=5 || peek(0xe0)||peek(0xe1)){previous_room=-1;return;}
    zelda_world_refresh();
    unsigned active=peek(0xeb);int x=peek(0x70),y=peek(0x84);
    capture(active);
    if(previous_room==(int)active && !zelda_world_portal(active)) {
        unsigned dir=0;int nx=x,ny=y;
        if(x-previous_x < -128)dir=1;
        else if(x-previous_x >128)dir=2;
        else if(y<64 && y<previous_y){dir=8;ny+=176;}
        else if(y>=240 && y>previous_y){dir=4;ny-=176;}
        int next=zelda_world_neighbor(active,dir);
        if(dir && next>=0 && adopt((unsigned)next,nx,ny)) {
            uint8_t palette[32];memcpy(palette,zelda_world_rooms[next].palette,32);
            active=(unsigned)next;x=nx;y=ny;capture(active);memcpy(zelda_world_rooms[active].palette,palette,32);
        }
    }
    previous_room=(int)active;previous_x=x;previous_y=y;
    /* Original room entry holds the incoming shutter open until Link clears
     * its doorway. Do that before the cross-boundary collision lookahead. */
    if(lev && peek(0x53)) {
        unsigned direction=peek(0x53),opposite=direction==1?2:direction==2?1:direction==4?8:4;
        int next=zelda_world_neighbor(active,direction);
        if(next>=0 && zelda_world_rooms[next].simulation_ready &&
           !(zelda_world_rooms[next].ram[0xee]&opposite))
            zelda_world_sync_doors((unsigned)next,opposite);
    }
    /* Visible region plus one room beyond the view. A full-map view can
     * activate every room; normal gameplay does not simulate distant rooms. */
    extern void zelda_world_sim_bounds(int *left,int *top,int *right,int *bottom);
    int l,t,r,b;zelda_world_sim_bounds(&l,&t,&r,&b);
    for(unsigned id=0;id<128;id++) {
        if(id==active || !zelda_world_rooms[id].owned)continue;
        int rx=(int)(id&15),ry=(int)(id>>4);
        if(rx>=l&&rx<=r&&ry>=t&&ry<=b)tick(id,active,x,y);
    }
}

static uint8_t tile_at(int wx,int wy) {
    if(wx<0||wx>=4096||wy<0||wy>=1408)return 0xff;
    unsigned id=(unsigned)(wy/176)*16+(unsigned)(wx/256);
    ZeldaWorldRoom *r=&zelda_world_rooms[id];
    if(!r->owned||!r->valid)return 0xff;
    unsigned x=(unsigned)(wx%256)/8,y=(unsigned)(wy%176)/8;
    return r->tiles[x*22+y];
}
static int hook(uint16_t pc) {
    if(!zelda_world_live())return 0;
    CycModRegs regs;cyc_mod_regs(&regs);
    if(pc==0xeda0) {
        if(!simulating)return 0;
        regs.x=0;cyc_mod_set_regs(&regs);return 1;
    }
    if(simulating || peek(0x12)!=5 || !zelda_world_supported())return 0;
    if(pc==0x917c) {
        /* Original door bounds stop at x=240/y=221. Seamless rooms include
         * the remaining border pixels, with the same native door gate. */
        unsigned x=peek(0x70),y=peek(0x84),edge=0,input=peek(0x3f8)&15;
        if(!peek(0x10)||regs.x)return 0;
        if(x==120&&y>221&&(input&12))edge=4;
        if(y==141&&x>240&&(input&3))edge=1;
        if(!edge)return 0;
        unsigned room=peek(0xeb),a=peek(0x687e + room),b=peek(0x68fe + room);
        unsigned type=edge==4?(a>>2)&7:(b>>2)&7;
        bool open=type==0||(type>=5&&(peek(0xee)&edge));
        if(!open||zelda_world_neighbor(room,edge)<0)return 0;
        poke(0xf,input);poke(0xe,0);poke(0x53,input);poke(0x98,input);
        return 1;
    }
    if(pc==0xf161) {
        if(zelda_world_portal(peek(0xeb)))return 0;
        /* The entrance room's south exit is a portal to the overworld. */
        if(peek(0x10)&&peek(0xeb)==peek(0x6bad)&&(peek(0x3f8)&4))return 0;
        regs.x=0;cyc_mod_set_regs(&regs);return 1;
    }
    if(regs.x || zelda_world_portal(peek(0xeb)))return 0;
    int x=peek(0x70),y=peek(0x84),dir=pc==0xedf4?0:peek(0xf);
    int tx=x,ty=y+11;
    if(dir&12)ty+=dir&4?8:-8;
    else if(dir)tx+=dir&1?16:-8;
    if(tx>=0&&tx<256&&ty>=64&&ty<240)return 0;
    unsigned room=peek(0xeb);
    int wx=(int)(room&15)*256+tx,wy=(int)(room>>4)*176+ty-64;
    uint8_t v=tile_at(wx,wy);
    if(dir&12){uint8_t other=tile_at(wx+8,wy);if(other>v)v=other;}
    if(!peek(0x10)) {
        const uint8_t walkable[]={0x8d,0x91,0x9c,0xac,0xad,0xcc,0xd2,0xd5,0xdf};
        for(unsigned i=0;i<sizeof walkable;i++)if(v==walkable[i]){v=0x26;break;}
    }
    poke(0x49e,v);regs.a=v;regs.y=peek(0x10);
    regs.p=(uint8_t)((regs.p&~0x82u)|(v&0x80)|(v?0:2));cyc_mod_set_regs(&regs);return 1;
}

/* Architectural state only; raster caches and debug counters are derived. */
enum { RECORD=1+2048+704+48+32+8, HEADER=40 };
static void put64(uint8_t *p,uint64_t n){for(int i=0;i<8;i++){p[i]=(uint8_t)n;n>>=8;}}
static uint64_t get64(const uint8_t *p){uint64_t n=0;for(int i=7;i>=0;i--)n=(n<<8)|p[i];return n;}
static int save(uint8_t *out,int cap) {
    if(!zelda_world_live())return 0;
    int need=HEADER+128*RECORD;if(cap<need)return -1;
    memset(out,0,(size_t)need);memcpy(out,"ZWR1",4);
    out[4]=(uint8_t)(level+1);out[5]=(uint8_t)(quest+1);out[6]=(uint8_t)(previous_room+1);
    out[7]=(uint8_t)previous_x;out[8]=(uint8_t)previous_y;
    put64(out+16,last_cycle);put64(out+24,handoffs);put64(out+32,failures);
    uint8_t *p=out+HEADER;
    for(unsigned id=0;id<128;id++) {
        const ZeldaWorldRoom *r=&zelda_world_rooms[id];
        *p++=r->simulation_ready?1:0;
        if(r->simulation_ready){memcpy(p,r->ram,2048);memcpy(p+2048,r->tiles,704);
            memcpy(p+2752,r->attrs,48);memcpy(p+2800,r->palette,32);put64(p+2832,r->ticks);}
        p+=RECORD-1;
    }
    return need;
}
static int validate(const uint8_t *in,int len) {
    if(!len)return 1;
    if(!in||len!=HEADER+128*RECORD||memcmp(in,"ZWR1",4)||in[4]>10||in[5]>3||in[6]>128)return 0;
    const uint8_t *p=in+HEADER;for(unsigned id=0;id<128;id++,p+=RECORD)if(*p>1)return 0;
    return 1;
}
static int load(const uint8_t *in,int len) {
    if(!validate(in,len))return 0;
    zelda_world_rooms_reset();
    for(unsigned id=0;id<128;id++){zelda_world_rooms[id].simulation_ready=false;zelda_world_rooms[id].valid=false;}
    if(!len)return 1;
    level=(int)in[4]-1;quest=(int)in[5]-1;previous_room=(int)in[6]-1;
    previous_x=in[7];previous_y=in[8];last_cycle=get64(in+16);handoffs=get64(in+24);failures=get64(in+32);
    const uint8_t *p=in+HEADER;
    for(unsigned id=0;id<128;id++) {
        ZeldaWorldRoom *r=&zelda_world_rooms[id];bool ready=*p++!=0;
        if(ready){memcpy(r->ram,p,2048);memcpy(r->tiles,p+2048,704);memcpy(r->attrs,p+2752,48);
            memcpy(r->palette,p+2800,32);r->ticks=get64(p+2832);r->simulation_ready=r->valid=true;}
        p+=RECORD-1;
    }
    return 1;
}
static void teleport(int id,const char *line) {
    long room=-1,x=120,y=141;
    cyc_tcp_long(line,"room",&room);cyc_tcp_long(line,"x",&x);cyc_tcp_long(line,"y",&y);
    if(!zelda_world_live()||peek(0x12)!=5||room<0||room>127||x<0||x>240||y<64||y>239){cyc_tcp_err(id,"world QA teleport needs gameplay, room0..127, x0..240, y64..239");return;}
    zelda_world_refresh();capture(peek(0xeb));
    if(!zelda_world_rooms[room].owned){cyc_tcp_err(id,"room does not belong to this area");return;}
    if(!adopt((unsigned)room,(int)x,(int)y)){cyc_tcp_err(id,"room preparation failed");return;}
    poke(0x394,0);poke(0xac,0);poke(0x4f0,120);
    bool gear=false;cyc_tcp_bool(line,"gear",&gear);
    if(gear){poke(0x657,1);poke(0x66f,0x77);poke(0x670,255);poke(0x658,8);poke(0x67c,8);poke(0x66e,1);}
    bool fragile=false;cyc_tcp_bool(line,"fragile",&fragile);
    if(fragile){poke(0x66f,0);poke(0x670,1);poke(0x4f0,0);}
    previous_room=(int)room;previous_x=(int)x;previous_y=(int)y;
    cyc_tcp_ok(id,"\"qa_teleport\":true");
}
static void room_state(int id,const char *line) {
    long room=peek(0xeb);cyc_tcp_long(line,"room",&room);
    if(room<0||room>127){cyc_tcp_err(id,"room0..127");return;}
    const ZeldaWorldRoom *r=&zelda_world_rooms[room];char out[3500];
    int n=snprintf(out,sizeof out,"\"room\":%ld,\"owned\":%s,\"ready\":%s,\"ticks\":%llu,\"world_position\":[%u,%u],\"slots\":[",room,r->owned?"true":"false",r->simulation_ready?"true":"false",(unsigned long long)r->ticks,
        (peek(0xeb)&15)*256+peek(0x70),(peek(0xeb)>>4)*176+peek(0x84)-64);
    for(unsigned s=1;s<=11;s++)n+=snprintf(out+n,sizeof out-(size_t)n,"%s{\"slot\":%u,\"type\":%u,\"x\":%u,\"y\":%u,\"state\":%u,\"meta\":%u,\"timer\":%u,\"hp\":%u}",s==1?"":",",s,r->ram[0x34f+s],r->ram[0x70+s],r->ram[0x84+s],r->ram[0xac+s],r->ram[0x405+s],r->ram[0x28+s],r->ram[0x485+s]);
    snprintf(out+n,sizeof out-(size_t)n,"]");cyc_tcp_ok(id,out);
}
static bool floor_tile(unsigned v) {
    if(!peek(0x10)) {
        const uint8_t extra[]={0x8d,0x91,0x9c,0xac,0xad,0xcc,0xd2,0xd5,0xdf};
        for(unsigned i=0;i<sizeof extra;i++)if(v==extra[i])return true;
    }
    return v<peek(0x34a);
}
static void edge_position(int id,const char *line) {
    long room=peek(0xeb),direction=1;cyc_tcp_long(line,"room",&room);cyc_tcp_long(line,"direction",&direction);
    if(room<0||room>127){cyc_tcp_err(id,"invalid room");return;}
    zelda_world_refresh();int next=zelda_world_neighbor((unsigned)room,(unsigned)direction);
    if(next<0){cyc_tcp_err(id,"no geographic neighbor");return;}
    int best=-1,distance=9999;const ZeldaWorldRoom *a=&zelda_world_rooms[room],*b=&zelda_world_rooms[next];
    for(int c=0;c<((direction&3)?22:31);c++) {
        bool free=false;
        if(direction==1)free=floor_tile(a->tiles[31*22+c])&&floor_tile(b->tiles[c]);
        if(direction==2)free=floor_tile(a->tiles[c])&&floor_tile(b->tiles[31*22+c]);
        if(direction==4)free=floor_tile(a->tiles[c*22+21])&&floor_tile(a->tiles[(c+1)*22+21])&&floor_tile(b->tiles[c*22])&&floor_tile(b->tiles[(c+1)*22]);
        if(direction==8)free=floor_tile(a->tiles[c*22])&&floor_tile(a->tiles[(c+1)*22])&&floor_tile(b->tiles[c*22+21])&&floor_tile(b->tiles[(c+1)*22+21]);
        int d=abs(c-((direction&3)?11:15));
        if(free&&d<distance){best=c;distance=d;}
    }
    if(best<0){cyc_tcp_err(id,"no aligned passable border in these room maps");return;}
    int x=(direction&3)?(direction==1?240:0):best*8;
    int y=(direction&3)?best*8+53:(direction==4?229:69);
    char out[200];snprintf(out,sizeof out,"\"room\":%ld,\"next\":%d,\"x\":%d,\"y\":%d",room,next,x,y);cyc_tcp_ok(id,out);
}
static void entrance_position(int id,const char *line) {
    long room=peek(0xeb);cyc_tcp_long(line,"room",&room);
    if(peek(0x10)||room<0||room>127){cyc_tcp_err(id,"overworld room required");return;}
    unsigned x=peek(0x687e + (unsigned)room)&0xf0;
    unsigned y=0x4d+(peek(0x6afe + (unsigned)room)&7)*16+16;
    unsigned destination=(peek(0x68fe + (unsigned)room)&0xfc)>>2;
    char out[160];snprintf(out,sizeof out,"\"x\":%u,\"y\":%u,\"destination\":%u",x,y,destination);cyc_tcp_ok(id,out);
}
void zelda_world_rooms_tcp_setup(void) {
    cyc_tcp_register("zelda_world_teleport","QA placement within current area",teleport);
    cyc_tcp_register("zelda_world_room","Room-local actor state and update count",room_state);
    cyc_tcp_register("zelda_world_edge","QA: find a real passable geographic seam",edge_position);
    cyc_tcp_register("zelda_world_entrance","QA: original overworld entrance approach",entrance_position);
}
NES_MOD_CONSTRUCTOR(register_zelda_world_rooms) {
    make_mask();
    for(unsigned i=0;i<sizeof hook_ids/sizeof *hook_ids;i++)
        if(!nes_mod_register_function_entry_plugin(hook_ids[i],hook_pcs[i],hook))abort();
    if(!nes_mod_register_savestate_hook("zelda.world.rooms",save,load)||
       !nes_mod_register_savestate_validator("zelda.world.rooms",validate))abort();
}
