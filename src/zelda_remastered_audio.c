/* Native observation of the retail sound driver. The Remastered download's
 * injected 6502 audio stubs are behavior references, never compiler inputs.
 * Guest sound requests, original driver state and hardware timing retain their
 * retail owners; only delivered PCM is replaced while this opt-in mod runs. */
#include "cyc_mod.h"
#include "hdpack_audio.h"
#include "mod_function_hooks.h"
#include "mod_runtime.h"
#include <stdlib.h>

static const char *const hooks[] = {
    "zelda.remastered.audio-mode",
    "zelda.remastered.audio-effect0",
    "zelda.remastered.audio-effect1",
    "zelda.remastered.audio-effect2",
    "zelda.remastered.audio-effect4",
    "zelda.remastered.audio-music",
    "zelda.remastered.audio-stop",
};
static const uint16_t addresses[] = {0x9825,0x9ae7,0x98d3,0x99a5,0x9b85,0x9c76,0x9d46};
static int active;

static void sound(unsigned a,unsigned track) {
    hdpack_audio_write(0x4104,(uint8_t)a);
    hdpack_audio_write(0x4106,(uint8_t)track);
}
static void music(unsigned track) {
    hdpack_audio_write(0x4100,track==2 || track==4 || track==6 || track==8?0:1);
    hdpack_audio_write(0x4104,3);
    hdpack_audio_write(0x4105,(uint8_t)track);
}
static int observe(uint16_t pc) {
    if (!active) return 0;
    if (pc==0x9d46) {hdpack_audio_write(0x4101,6);return 0;}
    if (pc==0x9c76) {
        CycModRegs regs;cyc_mod_regs(&regs);
        music(cyc_mod_peek(0x12)==1?3:regs.a);
        return 0;
    }
    CycModRegs regs;cyc_mod_regs(&regs);
    unsigned tick=cyc_mod_peek(0x15);
    if(pc==0x9825) {
        if(cyc_mod_peek(0xe0)) return 0;
        unsigned mode=cyc_mod_peek(0x12);
        if(mode==1 && hdpack_audio_current_music()!=0x303) music(3);
        else if(mode==2 && hdpack_audio_current_music()==0x303)
            hdpack_audio_write(0x4101,2);
    } else if(pc==0x9ae7) {
        sound(0,regs.a);
    } else if(pc==0x98d3) {
        if(regs.a!=0x40 || !(tick&0x1f)) sound(1,regs.a);
    } else if(pc==0x99a5) {
        if(regs.y && (regs.y!=0x20 || !(tick&0x7f))) sound(2,regs.y);
    } else if(pc==0x9b85) {
        unsigned request=cyc_mod_peek(0x601);
        if(!request) {
            request=cyc_mod_peek(0x608);
            if(!(request&0x80)) return 0;
        }
        if(request && (!(request&0x80) || !(tick&0x7f))) sound(4,request);
    }
    return 0; /* Original native driver, registers and requests are untouched. */
}
static void reset(void) {
    active=0;
    for(unsigned i=0;i<7;++i) nes_mod_set_function_hook_enabled(hooks[i],0);
    cyc_mod_set_apu_audio_mute(0);
}
static void activate(void) {
    active=1;
    for(unsigned i=0;i<7;++i)
        if(!nes_mod_set_function_hook_enabled(hooks[i],1)) abort();
    cyc_mod_set_apu_audio_mute(0x1f);
}
NES_MOD_CONSTRUCTOR(register_remastered_audio) {
    for(unsigned i=0;i<7;++i)
        if(!nes_mod_register_function_entry_plugin(hooks[i],addresses[i],observe)) abort();
    if(!nes_mod_register_reset_callback(reset) ||
       !nes_mod_register_activation_plugin("legend-of-zelda.remastered.audio",activate)) abort();
}
