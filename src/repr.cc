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


    // const char* type[2] = { "dimer", "generic" };
    // int idx, count = 0;
    // for (int i_type = 0; i_type < 2; i_type ++)
    //     if (config_lookup( cfg, type[i_type] ) != NULL) {
    //         count ++;
    //         idx = i_type;
    //     }

    // if (count == 0) {
    //     // nothing defined here, so it is the post-process
    // } else if (count == 1) { // normal calculation
    //     if (strcmp( type[idx], "dimer" ) == 0) {
    //         para_repr_dimer_local pdl;
    //         para_repr_dimer_config( cfg, &pdl );
    //         repr_set_exciton_dimer( ps, &pdl );
    //     }
    //     else if (strcmp( type[idx], "generic" ) == 0) {
    //         // para_repr_generic pg;
    //         para_repr_generic_config( cfg, ps );
    //         // repr_set_generic( ps, &pg ); // TODO: here we need to make sure it's right
    //     }
    // } else {
    //     error( ps, "%d", count );
    // }
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
