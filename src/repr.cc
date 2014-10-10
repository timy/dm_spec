#include "para.h"
#include "repr.h"
#include <cmath>
#include <cstdio>
#include <cassert>

void para_repr_config( config_t* cfg, parameters* ps );
void para_repr_dimer_config( config_t *cfg, para_repr_dimer_local *pdl );
void para_repr_generic_config( config_t *cfg, parameters* ps );

#include <cstring>
#include <libconfig.h>
#include "debug.h"

void para_repr_ini( config_t* cfg, parameters* ps )
{
    ps->repr = new para_repr;
    para_repr_config( cfg, ps );

    if (ps->repr->type == para_repr::GENERIC) {
        para_repr_generic_config( cfg, ps );
    } else if (ps->repr->type == para_repr::DIMER) {
        ps->repr->ptrSt = new para_repr_dimer_local;
        para_repr_dimer_config( cfg, (para_repr_dimer_local*) ps->repr->ptrSt );
        repr_set_exciton_dimer( ps, (para_repr_dimer_local*) ps->repr->ptrSt );
    }
}

void para_repr_del( parameters *ps )
{
    if (ps->repr->type == para_repr::GENERIC) {
    } else if (ps->repr->type == para_repr::DIMER) {
        delete ps->repr->ptrSt;
    }
    delete ps->repr;
}

#include <libconfig.h>
#include <cstring>
void para_repr_config( struct config_t* cfg, struct parameters* ps )
{
    const char* name = NULL;
    config_lookup_string( cfg, "repr.type", &name );
    if (strcmp(name, "generic") == 0) {
        ps->repr->type = para_repr::GENERIC;
    } else if (strcmp(name, "dimer") == 0) {
        ps->repr->type = para_repr::DIMER;
    } else {
        fprintf( stderr, "repr.type '%s' unknown. Check "
                 "section 'repr' in your configure file\n", name );
    }
}
