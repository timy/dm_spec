#include "para.h"
#include "pols.h"

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
void para_pols_config( struct config_t* cfg, struct parameters *ps )
{
    ps->pols->bPolForEachDpl = false;
    config_lookup_bool( cfg, "pols.b_pol_for_each_dpl", &(ps->pols->bPolForEachDpl) );
    if (ps->pols->bPolForEachDpl == true) {
        ps->pols->n_dpl = ps->n_dpl;
    } else {
        ps->pols->n_dpl = 1;
    }
}
