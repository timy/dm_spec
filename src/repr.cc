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
        para_repr_dimer_local pdl;
        para_repr_dimer_config( cfg, &pdl );
        repr_set_exciton_dimer( ps, &pdl );
    }
}

void para_repr_del( parameters *para )
{
    delete para->repr;
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
