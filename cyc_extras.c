/* Original cycle CPU and guest input; Zelda's voxel profile changes only the
 * displayed scene and, in first-person, the player's direction mapping. */
#include "cyc_host_extras.h"
#include "cyc_render.h"
#include "cyc_video.h"
#include "cyc_tcp.h"
#include "cycle_bridge.h"
#include "zelda_voxel.h"
#include "keybinds.h"
#ifdef NESRECOMP_CYCLE_HDPACK
#include "cyc_hdpack.h"
#endif
#include <string.h>
#include <stdio.h>

static uint32_t picture[CYC_VIDEO_MAX_WIDTH * 240];
static void power_on(void *ctx) {
    (void)ctx;
    cyc_presentation_sync();
    char path[1024];char *base=SDL_GetBasePath();
    snprintf(path,sizeof path,"%sLegendOfZeldaNESRecomp.exe",base?base:"");SDL_free(base);
    keybinds_init_readonly(path);
    zelda_voxel_init();
#ifdef NESRECOMP_CYCLE_HDPACK
    cyc_hdpack_power_on();
#endif
}
static void input(void *ctx, uint8_t buttons[2]) {
    (void)ctx;
    cyc_presentation_sync();
    g_controller1_buttons=buttons[0];
    zelda_voxel_update_hotkey();
    buttons[0]=g_controller1_buttons;
}
static void event(void *ctx, const void *ev, int player) {
    (void)ctx;
    cyc_presentation_event((const SDL_Event *)ev,player);
    zelda_voxel_handle_event((const SDL_Event *)ev);
}
static void frame_end(void *ctx) {
    (void)ctx;
    cyc_presentation_sync();
    int width,height;const uint32_t *native=cyc_render_present(&width,&height);
    memcpy(picture,native,(size_t)width*height*sizeof(*picture));
    zelda_voxel_post_render(picture);
}
static const uint32_t *present(void *ctx,int *width,int *height) {
    /* Rebuild after a load as well. The profile's stable-room cache is part
     * of its validated save record, so room transitions resume unchanged. */
    frame_end(ctx);
#ifdef NESRECOMP_CYCLE_HDPACK
    return cyc_hdpack_present(picture,width,height);
#endif
    *width=g_render_width;*height=240;
    return picture;
}
static bool option(void *ctx,const char *name,const char *value) {
    (void)ctx;(void)name;
#ifdef NESRECOMP_CYCLE_HDPACK
    if(!strcmp(name,"--hdpack")){cyc_hdpack_config(strcmp(value,"off")!=0,!strcmp(value,"off")?"":value);return true;}
    if(strcmp(value,"stock"))return false;
#endif
    if(!strcmp(value,"stock"))zelda_voxel_set_mod_enabled(0);
    else if(!strcmp(value,"diorama") || !strcmp(value,"first-person")) {
        int fp=!strcmp(value,"first-person");
        zelda_voxel_configure_mod(fp,fp?0:35,fp?0:-20,0,100,fp?115:135);
        zelda_voxel_set_mod_enabled(1);
    } else return false;
    zelda_voxel_init();return true;
}
static void state(int id,const char *line) {
    (void)line;cyc_presentation_sync();
    char out[1000];
    snprintf(out,sizeof out,"\"mode\":%u,\"room\":%u,\"link_x\":%u,\"link_y\":%u,\"direction\":%u,\"link_state\":%u,\"hearts\":%u,\"sword\":%u,\"paused\":%u,\"submenu\":%u,\"voxel\":%d,\"width\":%d",
        g_ram[0x12],g_ram[0xeb],g_ram[0x70],g_ram[0x84],g_ram[0x98],g_ram[0xac],g_ram[0x66f],g_ram[0x657],g_ram[0xe0],g_ram[0xe1],zelda_voxel_cycle_mode(),g_render_width);
    size_t used=strlen(out);
    snprintf(out+used,sizeof out-used,",\"game_mode\":%u,\"hp\":%u,\"sub_mode\":%u",g_ram[0x12],g_ram[0x66f],g_ram[0x13]);
    cyc_tcp_ok(id,out);
}
static void entities(int id,const char *line) {
    (void)line;cyc_presentation_sync();char out[3000];
    int n=snprintf(out,sizeof out,"\"frame\":%llu,\"mode\":%u,\"sub\":%u,\"slots\":[",(unsigned long long)g_frame_count,g_ram[0x12],g_ram[0x11]);
    for(int i=0;i<12;i++)n+=snprintf(out+n,sizeof out-(size_t)n,"%s{\"i\":%d,\"type\":%u,\"x\":%u,\"y\":%u,\"dir\":%u,\"state\":%u,\"meta\":%u,\"timer\":%u,\"uninit\":%u}",
        i?",":"",i,g_ram[0x34f+i],g_ram[0x70+i],g_ram[0x84+i],g_ram[0x98+i],g_ram[0xac+i],g_ram[0x405+i],g_ram[0x28+i],g_ram[0x492+i]);
    snprintf(out+n,sizeof out-(size_t)n,"]");cyc_tcp_ok(id,out);
}
static void entity(int id,const char *line) {
    long slot=1;cyc_tcp_long(line,"slot",&slot);if(slot<0 || slot>11)slot=1;
    cyc_presentation_sync();char out[1000];
    snprintf(out,sizeof out,"\"frame\":%llu,\"slot\":%ld,\"type\":\"0x%02X\",\"x\":%u,\"y\":%u,\"dir\":%u,\"state\":\"0x%02X\",\"meta\":%u,\"timer\":%u,\"uninit\":\"0x%02X\",\"stun_cycle\":%u,\"paused\":%u,\"menu_state\":%u,\"mode\":%u,\"sub\":%u",
        (unsigned long long)g_frame_count,slot,g_ram[0x34f+slot],g_ram[0x70+slot],g_ram[0x84+slot],g_ram[0x98+slot],g_ram[0xac+slot],g_ram[0x405+slot],g_ram[0x28+slot],g_ram[0x492+slot],g_ram[0x26],g_ram[0xe0],g_ram[0xe1],g_ram[0x12],g_ram[0x11]);cyc_tcp_ok(id,out);
}
static void tcp_setup(void *ctx) {
    (void)ctx;cyc_tcp_register("zelda_state","Guest state and voxel view",state);
    cyc_tcp_register("entity_snapshot","Original Zelda entity slots",entities);
    cyc_tcp_register("entity_slot","Detailed original entity slot",entity);
}
static const CycHostOption options[]={
    {"--voxel",true,"stock, diorama or first-person"},
#ifdef NESRECOMP_CYCLE_HDPACK
    {"--hdpack",true,"Remastered texture pack directory, or off"},
#endif
};
const CycHostExtras *cyc_host_extras(void) {
    static const CycHostExtras extras={.present=present,.power_on=power_on,.frame_end=frame_end,
        .input=input,.event=event,.options=options,.option_count=sizeof(options)/sizeof(options[0]),.option=option,.tcp_setup=tcp_setup};
    return &extras;
}
