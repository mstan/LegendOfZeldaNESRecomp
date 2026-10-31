/* Full final-canvas differential against a scalar world-pixel oracle. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fail_alloc;
static void *test_realloc(void *p,size_t bytes) {return fail_alloc?NULL:realloc(p,bytes);}
#define realloc test_realloc
#include "../src/zelda_world_canvas.c"
#undef realloc

static uint32_t rasters[128*256*176];
static uint32_t expected[4096*2048],actual[4096*2048],alternate[4096*2048];
static uint8_t flags[128];
static void oracle(uint32_t *out,const ZeldaCanvasView *v) {
    for(int y=0;y<v->height;y++)for(int x=0;x<v->width;x++) {
        int wx=v->camera_x+x,wy=v->camera_y+y-64;
        uint32_t color=v->background;
        if(y>=64&&wx>=0&&wx<4096&&wy>=0&&wy<1408) {
            unsigned id=(unsigned)(wy/176)*16+(unsigned)(wx/256);
            if(v->valid[id])color=v->rasters[id*256*176+(wy%176)*256+wx%256];
        }
        out[(size_t)y*v->width+x]=color;
    }
}
static void overlays(uint32_t *out,const ZeldaCanvasView *v,int frame,int record) {
    /* HUD overwrites every top-row pixel; actors are transparent, clipped,
     * overlapping cards that move, change shape, disappear and reappear. */
    for(int y=0;y<64&&y<v->height;y++)for(int x=0;x<v->width;x++)
        out[(size_t)y*v->width+x]=0xff000000u+(unsigned)frame*17u+(unsigned)x;
    for(int i=0;i<37;i++) {
        if((frame+i)%7==0)continue;
        int x=(frame*13+i*53)%(v->width+20)-12;
        int y=(frame*7+i*29)%(v->height+20)-4;
        if(record)zelda_canvas_dirty(x,y,8,16);
        for(int dy=0;dy<16;dy++)for(int dx=0;dx<8;dx++) {
            int px=x+dx,py=y+dy;
            if(px<0||px>=v->width||py<64||py>=v->height||(dx+dy+frame)%3==0)continue;
            out[(size_t)py*v->width+px]=0xffe02080u+(unsigned)i*4u;
        }
    }
}
static int check(uint32_t *canvas,ZeldaCanvasView *v,int frame,int retain) {
    oracle(expected,v);overlays(expected,v,frame,0);
    zelda_canvas_begin(canvas,v,retain!=0);overlays(canvas,v,frame,1);
    size_t count=(size_t)v->width*v->height;
    if(memcmp(canvas,expected,count*sizeof(uint32_t))) {
        for(size_t i=0;i<count;i++)if(canvas[i]!=expected[i]) {
            fprintf(stderr,"frame%d pixel%zu %08x != %08x\n",frame,i,canvas[i],expected[i]);break;
        }
        return 0;
    }
    return 1;
}
int main(void) {
    for(unsigned i=0;i<128*256*176;i++)rasters[i]=0xff000000u|(i*2654435761u&0xffffffu);
    memset(flags,1,sizeof(flags));
    ZeldaCanvasView v={426,240,0,0,0xff102030,rasters,flags,1};
    uint32_t *canvas=actual;
    uint64_t before_revision_builds=0;
    for(int frame=0;frame<96;frame++) {
        if(frame<8){v.camera_x=frame*11;v.camera_y=frame*7;}
        if(frame==16){v.width=4096;v.height=1472;v.camera_x=v.camera_y=0;}
        if(frame==24){v.width=426;v.height=240;v.camera_x=900;v.camera_y=240;}
        if(frame==32){flags[20]=0;}
        if(frame==40){
            /* Room19 local(180,100) is viewport(48,100), visible and not
             * covered by this frame's actor cards. The previous frame reused. */
            uint64_t before_reuses=0;
            zelda_canvas_stats(&before_revision_builds,&before_reuses,NULL);
            if(!before_reuses)return 1;
            rasters[19*256*176+100*256+180]^=0x00ffffff;v.revision++;
        }
        if(frame==44){v.background^=0x00700f30;}
        if(frame==48){v.camera_x=-100;v.camera_y=-75;}
        if(frame==56){v.camera_x=4000;v.camera_y=1350;}
        if(frame==60){canvas=alternate;}
        if(frame==64){
            memset(canvas,0x91,(size_t)v.width*v.height*sizeof(uint32_t));
            zelda_canvas_invalidate(); /* inventory/game-over/portal overwrite */
        }
        if(frame==72){flags[20]=1;v.camera_x=900;v.camera_y=240;}
        if(!check(canvas,&v,frame,frame<84||frame>=88))return 1;
        if(frame==40){
            uint64_t after_builds=0;zelda_canvas_stats(&after_builds,NULL,NULL);
            if(after_builds<=before_revision_builds||canvas[100*426+48]!=rasters[19*256*176+100*256+180]){
                fputs("visible raster revision did not rebuild the viewport\n",stderr);return 1;
            }
        }
        if(frame==80){for(int i=0;i<8193;i++)zelda_canvas_dirty(1,65,8,16);}
    }
    uint64_t b,r,bytes;zelda_canvas_stats(&b,&r,&bytes);
    if(!b||!r||bytes>32u*1024u*1024u)return 1;
    zelda_canvas_release();fail_alloc=1;
    if(!check(canvas,&v,96,1)||!check(canvas,&v,97,1))return 1;
    fail_alloc=0;
    if(!check(canvas,&v,98,1)||!check(canvas,&v,99,1))return 1;
    zelda_canvas_release();
    puts("PASS:100 full canvases; Fit/atlas, motion/transparency/clipping, HUD, palette/raster/visibility, camera/edges, pointer/resize/UI invalidation, overflow and allocation recovery");
    return 0;
}
