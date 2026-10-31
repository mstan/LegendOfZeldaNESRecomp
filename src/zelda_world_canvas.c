/* Retain the complete terrain viewport, then restore only the rectangles that
 * the previous actor pass may have painted. Current actors and HUD still draw
 * normally. All storage here is disposable host presentation state. */
#include "zelda_world_canvas.h"
#include <stdlib.h>
#include <string.h>

enum { ROOM_W=256,ROOM_H=176,HUD_H=64,ROOMS=128,
       WORLD_W=4096,WORLD_H=1408,MAX_DIRTY=ROOMS*64 };
typedef struct {int left,top,right,bottom;} DirtyRect;
static uint32_t *terrain;
static size_t capacity;
static ZeldaCanvasView previous;
static uint32_t *previous_canvas;
static uint8_t previous_valid[ROOMS];
static DirtyRect dirty[MAX_DIRTY];
static unsigned dirty_count;
static bool valid,active;
static uint64_t builds,reuses;

void zelda_canvas_invalidate(void) {
    valid=active=false;dirty_count=0;
}
void zelda_canvas_release(void) {
    free(terrain);terrain=NULL;capacity=0;
    zelda_canvas_invalidate();previous_canvas=NULL;builds=reuses=0;
}
void zelda_canvas_stats(uint64_t *b,uint64_t *r,uint64_t *bytes) {
    if(b)*b=builds;
    if(r)*r=reuses;
    if(bytes)*bytes=capacity*sizeof(uint32_t);
}

static void compose(uint32_t *canvas,const ZeldaCanvasView *v) {
    size_t count=(size_t)v->width*v->height;
    for(size_t i=0;i<count;i++)canvas[i]=v->background;
    for(int y=HUD_H;y<v->height;y++) {
        int wy=v->camera_y+y-HUD_H;
        if(wy<0||wy>=WORLD_H)continue;
        for(int x=0;x<v->width;) {
            int wx=v->camera_x+x;
            if(wx<0){x++;continue;}
            if(wx>=WORLD_W)break;
            unsigned id=(unsigned)(wy/ROOM_H)*16+(unsigned)(wx/ROOM_W);
            int run=ROOM_W-wx%ROOM_W;
            if(run>v->width-x)run=v->width-x;
            if(v->valid[id])
                memcpy(canvas+(size_t)y*v->width+x,
                       v->rasters+(size_t)id*ROOM_W*ROOM_H+
                           (wy%ROOM_H)*ROOM_W+wx%ROOM_W,
                       (size_t)run*sizeof(uint32_t));
            x+=run;
        }
    }
}

void zelda_canvas_begin(uint32_t *canvas,const ZeldaCanvasView *v,bool retain) {
    if(!canvas||!v||!v->rasters||!v->valid||v->width<=0||v->width>4096||
       v->height<=0||v->height>2048) {
        zelda_canvas_invalidate();return;
    }
    bool same=retain&&valid&&canvas==previous_canvas&&
        v->width==previous.width&&v->height==previous.height&&
        v->camera_x==previous.camera_x&&v->camera_y==previous.camera_y&&
        v->background==previous.background&&v->rasters==previous.rasters&&
        v->revision==previous.revision&&!memcmp(v->valid,previous_valid,ROOMS);
    if(same) {
        for(unsigned i=0;i<dirty_count;i++) {
            DirtyRect r=dirty[i];
            for(int y=r.top;y<r.bottom;y++)
                memcpy(canvas+(size_t)y*v->width+r.left,
                       terrain+(size_t)y*v->width+r.left,
                       (size_t)(r.right-r.left)*sizeof(uint32_t));
        }
        dirty_count=0;active=true;reuses++;
        return;
    }
    zelda_canvas_invalidate();compose(canvas,v);
    if(!retain)return;
    builds++;
    size_t count=(size_t)v->width*v->height;
    if(count>capacity) {
        uint32_t *next=realloc(terrain,count*sizeof(uint32_t));
        if(!next)return; /* Current full composition already succeeded. */
        terrain=next;capacity=count;
    }
    memcpy(terrain,canvas,count*sizeof(uint32_t));
    previous=*v;previous.valid=NULL;previous_canvas=canvas;
    memcpy(previous_valid,v->valid,ROOMS);
    valid=active=true;
}

void zelda_canvas_dirty(int x,int y,int width,int height) {
    if(!active||width<=0||height<=0)return;
    int64_t right=(int64_t)x+width,bottom=(int64_t)y+height;
    int left=x<0?0:x,top=y<HUD_H?HUD_H:y;
    if(right>previous.width)right=previous.width;
    if(bottom>previous.height)bottom=previous.height;
    if(right<=left||bottom<=top)return;
    if(dirty_count==MAX_DIRTY){zelda_canvas_invalidate();return;}
    dirty[dirty_count++]=(DirtyRect){left,top,(int)right,(int)bottom};
}
