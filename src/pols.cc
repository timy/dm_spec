#include "para.h"
#include "pols.h"
#include "debug.h"
void para_pols_config( struct config_t* cfg, struct parameters *ps );

void para_pols_ini( struct config_t* cfg, struct parameters *ps )
{
    ps->pols = new para_pols;
    para_pols_config( cfg, ps );
    set_para_pols( ps );
}

void para_pols_del( struct parameters *ps )
{
    delete ps->pols;
}

void set_para_pols( struct parameters *ps )
{
}

#include <libconfig.h>
#include <cstring>
void para_pols_config( struct config_t* cfg, struct parameters *ps )
{
    const char* name = NULL;
    config_lookup_string( cfg, "pols.ppar_calc_method", &name );
    if (strcmp(name, "pullerits") == 0) {
        ps->pols->method_ppar = para_pols::PULLERITS;
    } else if (strcmp(name, "seidner") == 0) {
        ps->pols->method_ppar = para_pols::SEIDNER;
    } else {
        fprintf( stderr, "method_ppar '%s' unknown. Check "
                 "section 'pols' in your configure file\n", name );
    }

    ps->pols->bPolForEachDpl = false;
    config_lookup_bool( cfg, "pols.b_pol_for_each_dpl", &(ps->pols->bPolForEachDpl) );
    if (ps->pols->bPolForEachDpl == true) {
        if (ps->pols->method_ppar == para_pols::PULLERITS) {
            print( "%s L%d Warning: for Pullerits' method, "
                   "better set b_pol_for_each_dpl=false\n" );
        }
        ps->pols->n_dpl = ps->n_dpl;
    } else {
        ps->pols->n_dpl = 1;
    }
}
