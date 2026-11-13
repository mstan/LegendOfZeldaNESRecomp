#include "mod_runtime.h"
#include <stdio.h>

static void activate_redux(void) { nes_mod_set_local_only("Zelda Redux",1); }
NES_MOD_CONSTRUCTOR(register_zelda_redux) {
    const char *ids[]={"zelda.redux.base","zelda.redux.like-like-rupees",
                      "zelda.redux.dungeon-themes","zelda.redux.title-screen"};
    for (unsigned i=0;i<sizeof(ids)/sizeof(ids[0]);i++)
        if (!nes_mod_register_activation_plugin(ids[i],activate_redux))
            fprintf(stderr,"Cannot register %s\n",ids[i]);
}
