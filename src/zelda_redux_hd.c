/* Game-owned presentation correspondence for the supported Remastered pack.
 * Pattern pairs are generated from the verified cartridges, never guessed at
 * runtime. This changes HD lookup metadata only; native PPU/CPU/APU stay intact. */
#include "cyc_host_extras.h"
#include "cyc_program.h"
#include "cycle_bridge.h"
#include "redux_hd_aliases.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned profile;
static void pattern(void *ctx,const uint8_t source[16],uint8_t key[16]) {
    (void)ctx;unsigned lo=0,hi=zelda_hd_alias_counts[profile];
    const ZeldaHdAlias *aliases=zelda_hd_aliases[profile];
    while(lo<hi) {
        unsigned mid=lo+(hi-lo)/2;int cmp=memcmp(source,aliases[mid].source,16);
        if(cmp<0)hi=mid;else if(cmp>0)lo=mid+1;
        else {memcpy(key,aliases[mid].key,16);return;}
    }
    memcpy(key,source,16);
}
static void palette(void *ctx,uint8_t pal[4]) {
    (void)ctx;
    /* Redux's heart outline and dungeon Link colors. Native fallback colors
     * still use the actual palette; these are only the pack's lookup keys. */
    if(pal[1]==0x16 && (pal[2]==0x27 || pal[2]==0x28) && pal[3]==0x30) {
        pal[2]=0x27;pal[3]=0x36;
    } else if(pal[1]==0x29 && pal[2]==0x27 && pal[3]==0x17)pal[2]=0x37;
}
static bool visible(void *ctx) {
    (void)ctx;if(!g_ram)return false;unsigned mode=g_ram[0x12];
    /* Redux's title, new inventory and native save menus keep their authored
     * layout. Gameplay, room transitions and caves use compatible HD art. */
    return ((mode>=4 && mode<=7) || mode==11) && !g_ram[0xe0];
}
static bool rules(void *ctx,const char *directory,char **out) {
    (void)ctx;*out=NULL;char path[1200];
    if(snprintf(path,sizeof path,"%s/hires.txt",directory)>=(int)sizeof path)return false;
    FILE *f=fopen(path,"rb");if(!f)return false;
    if(fseek(f,0,SEEK_END)){fclose(f);return false;}
    long length=ftell(f);rewind(f);
    if(length!=6137824){fclose(f);return false;}
    char *source=malloc((size_t)length+1),*result=malloc((size_t)length+1);
    if(!source || !result){free(source);free(result);fclose(f);return false;}
    bool ok=fread(source,1,(size_t)length,f)==(size_t)length && !ferror(f);fclose(f);
    uint64_t hash=UINT64_C(14695981039346656037);
    for(long i=0;ok && i<length;i++)hash=(hash^(uint8_t)source[i])*UINT64_C(1099511628211);
    /* Exact upstream 1.0.6 rules, SHA256
     * 8b734893e21adf36d0798e0cd21aef05b0b11b780d86afa2795fef443a6106ba. */
    if(!ok || hash!=UINT64_C(0x440c1d4273ad9ccb)){free(source);free(result);return false;}
    source[length]=0;size_t used=0;unsigned guards=0,hud=0;
    for(char *line=source;*line;) {
        char *end=strchr(line,'\n');if(end)*end=0;
        if(strstr(line,"<background>hud.png,")){hud++;}
        else {
            const char *text=line;
            if(!strncmp(line,"<condition>TitleGlitchFix1,",sizeof("<condition>TitleGlitchFix1,")-1)) {
                text="<condition>TitleGlitchFix1,memoryCheckConstant,12,>=,4";guards++;
            } else if(!strncmp(line,"<condition>OverworldMapHudCheck,",sizeof("<condition>OverworldMapHudCheck,")-1))
                text="<condition>OverworldMapHudCheck,memoryCheckConstant,10,==,0";
            size_t n=strlen(text);if(n+1>(size_t)length-used){ok=false;break;}
            memcpy(result+used,text,n);used+=n;result[used++]='\n';
        }
        if(!end)break;line=end+1;
    }
    free(source);
    if(!ok || guards!=1 || hud!=1){free(result);return false;}
    result[used]=0;*out=result;
    fprintf(stderr,"[Redux HD] qualified Remastered1.0.6 rules; profile%u, %u pattern correspondences; native Redux HUD layout\n",profile,zelda_hd_alias_counts[profile]);
    return true;
}
const CycHdPackAdapter *zelda_redux_hd_adapter(void *ctx) {
    (void)ctx;const CycProgram *p=cyc_program_current();
    static const char prefix[]="zelda_redux_";
    if(!p || strncmp(p->id,prefix,sizeof(prefix)-1))return NULL;
    const char *suffix=p->id+sizeof(prefix)-1;
    if(suffix[0]<'0' || suffix[0]>'7' || suffix[1])return NULL;
    profile=(unsigned)(suffix[0]-'0');
    static const CycHdPackAdapter adapter={rules,pattern,palette,visible,64};return &adapter;
}
