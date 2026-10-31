/* ROM-backed differential oracle; compiled only with ZELDA_TEST_NATIVE_JOBS.
 * The reference executes the real copied routine through its normal CPU path.
 * No duplicate host implementation supplies expected results. */
#include "cyc_mod.h"
#include "cyc_core.h"
#include "cyc_state.h"
#include "cyc_tcp.h"
#include "hw_internal.h"
#include "cpu6502.h"
#include "mod_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void zelda_world_test_sprite_mode(bool sim,bool enabled);
extern uint64_t zelda_world_test_sprite_jobs(void);
typedef struct {
    bool ok;
    CycModRegs regs;
    uint8_t ram[2048];
    uint64_t hardware,cycles,jobs;
} Result;
static bool run(Result *out,unsigned value,unsigned budget,bool native,int mutate) {
    if(!cyc_mod_isolate_begin())return false;
    cyc_mod_allow_isolated_hooks(true);
    zelda_world_test_sprite_mode(true,native);
    cyc_mod_set_call_budget(budget);
    for(unsigned a=0x200;a<0x380;a++)cyc_mod_poke((uint16_t)a,(uint8_t)(a*31+value));
    cyc_mod_poke(0x342,(uint8_t)value);
    if(mutate>=0) {
        uint16_t a=mutate<25?(uint16_t)(0x6e1d+mutate):(uint16_t)(0x6e39+mutate-25);
        cyc_mod_poke(a,(uint8_t)(cyc_mod_peek(a)^1));
    }
    memset(&out->regs,0,sizeof out->regs);
    out->regs.a=(uint8_t)(value^0xff);out->regs.x=(uint8_t)(value+7);
    out->regs.y=(uint8_t)(value^0xa5);out->regs.s=(uint8_t)(value^0xfd);
    out->regs.p=(uint8_t)value;
    uint64_t jobs=zelda_world_test_sprite_jobs();
    out->ok=cyc_mod_call(0x6e1d,&out->regs);
    out->cycles=hw_isolated_cycles;out->jobs=zelda_world_test_sprite_jobs()-jobs;
    memcpy(out->ram,cyc_cpu_ram(),sizeof out->ram);
    out->hardware=cyc_hw_state_hash();
    zelda_world_test_sprite_mode(false,true);
    cyc_mod_isolate_end();cyc_mod_set_call_budget(2000000);
    return true;
}
static bool equal(const Result *a,const Result *b) {
    return a->ok==b->ok && a->cycles==b->cycles && a->hardware==b->hardware &&
        a->regs.a==b->regs.a && a->regs.x==b->regs.x && a->regs.y==b->regs.y &&
        a->regs.p==b->regs.p && a->regs.s==b->regs.s && a->regs.pc==b->regs.pc &&
        !memcmp(a->ram,b->ram,sizeof a->ram);
}
static void noop_return(void) {}
static uint8_t noop_read(uint16_t pc,uint16_t addr,uint8_t value) {(void)pc;(void)addr;return value;}
static void oracle(int id,const char *line) {
    (void)line;
    static Result a,b;
    unsigned cases=0,mutations=0,guards=0;
    char message[160];
    for(unsigned v=0;v<256;v++) {
        if(!run(&a,v,2000000,false,-1)||!run(&b,v,2000000,true,-1)||
           !equal(&a,&b)||!b.ok||b.jobs!=1) {
            snprintf(message,sizeof message,"counter %u: ok %u/%u cycles %llu/%llu jobs %llu hw %llx/%llx",v,a.ok,b.ok,(unsigned long long)a.cycles,(unsigned long long)b.cycles,(unsigned long long)b.jobs,(unsigned long long)a.hardware,(unsigned long long)b.hardware);
            cyc_tcp_err(id,message);return;
        }
        if(a.cycles!=(v==39?763u:762u)){cyc_tcp_err(id,"reference cycle contract differs");return;}
        cases++;
    }
    static const unsigned limits[]={1,751,752,753,761,762,763};
    static const unsigned values[]={38,39,40,127,255};
    for(unsigned v=0;v<sizeof values/sizeof *values;v++)
        for(unsigned n=0;n<sizeof limits/sizeof *limits;n++) {
            if(!run(&a,values[v],limits[n],false,-1)||!run(&b,values[v],limits[n],true,-1)||!equal(&a,&b)) {
                snprintf(message,sizeof message,"budget %u counter %u",limits[n],values[v]);cyc_tcp_err(id,message);return;
            }
            cases++;
        }
    for(int i=0;i<38;i++) {
        if(!run(&a,39,1000,false,i)||!run(&b,39,1000,true,i)||!equal(&a,&b)||b.jobs) {
            snprintf(message,sizeof message,"content mutation %d",i);cyc_tcp_err(id,message);return;
        }
        mutations++;
    }
    if(!cyc_mod_isolate_begin()){cyc_tcp_err(id,"guard scope");return;}
    cyc_mod_allow_isolated_hooks(true);
    hw_isolated_cycles=0;hw_isolated_budget=100;cpu.pc=0x6e1d;
    uint8_t byte=cyc_mod_peek(0x6e1d);
    CycModCodeRegion code={0x6e1d,&byte,1};
    bool good=cyc_mod_isolated_job_ready(&code,1,10);
    hw.irq_line=1;good=good&&!cyc_mod_isolated_job_ready(&code,1,10);hw.irq_line=0;guards++;
    cpu.do_irq=1;good=good&&!cyc_mod_isolated_job_ready(&code,1,10);cpu.do_irq=0;guards++;
    cyc_mod_set_return_hook(noop_return);good=good&&!cyc_mod_isolated_job_ready(&code,1,10);cyc_mod_set_return_hook(NULL);guards++;
    cyc_mod_set_ram_read_hook(noop_read);good=good&&!cyc_mod_isolated_job_ready(&code,1,10);cyc_mod_set_ram_read_hook(NULL);guards++;
    uint8_t readable=hw_cart.wram_readable;hw_cart.wram_readable=0;
    good=good&&!cyc_mod_isolated_job_ready(&code,1,10);hw_cart.wram_readable=readable;guards++;
    uint8_t chr0=hw_cart.m.chr0;hw_cart.m.chr0|=16;
    good=good&&!cyc_mod_isolated_job_ready(&code,1,10);hw_cart.m.chr0=chr0;guards++;
    cyc_mod_isolate_end();
    if(!good){cyc_tcp_err(id,"memory job guard refusal");return;}
    snprintf(message,sizeof message,"\"cases\":%u,\"mutations\":%u,\"guards\":%u",cases,mutations,guards);
    cyc_tcp_ok(id,message);
}
static void report_jobs(void) {
    fprintf(stderr,"NES10_NATIVE_SPRITE jobs=%llu\n",(unsigned long long)zelda_world_test_sprite_jobs());
}
NES_MOD_CONSTRUCTOR(register_sprite_job_oracle) {
    cyc_tcp_register("zelda_test_native_sprite","ROM-backed isolated sprite-service oracle",oracle);
    atexit(report_jobs);
}
