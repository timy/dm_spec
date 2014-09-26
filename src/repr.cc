#include "para.h"
#include "repr.h"
#include <cmath>
#include <cstdio>
#include <cassert>

void para_repr_dimer_config( config_t *cfg, para_repr_dimer_local *pdl );
void para_repr_generic_config( config_t *cfg, parameters* ps );

#include <cstring>
#include <libconfig.h>
#include "debug.h"

void para_repr_ini( config_t* cfg, parameters* ps )
{
    ps->repr = new para_repr;

    const char* type[2] = { "dimer", "generic" };
    int idx, count = 0;
    for (int i_type = 0; i_type < 2; i_type ++)
        if (config_lookup( cfg, type[i_type] ) != NULL) {
            count ++;
            idx = i_type;
        }

    if (count == 0) {
        // nothing defined here, so it is the post-process
    } else if (count == 1) { // normal calculation
        if (strcmp( type[idx], "dimer" ) == 0) {
            para_repr_dimer_local pdl;
            para_repr_dimer_config( cfg, &pdl );
            repr_set_exciton_dimer( ps, &pdl );
        }
        else if (strcmp( type[idx], "generic" ) == 0) {
            // para_repr_generic pg;
            para_repr_generic_config( cfg, ps );
            // repr_set_generic( ps, &pg ); // TODO: here we need to make sure it's right
        }
    } else {
        error( ps, "%d", count );
    }
}

void para_repr_del( parameters *para )
{
    delete para->repr;
}
