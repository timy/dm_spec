#include "position.h"
#include "para.h"

void para_pos_config( config_t* cfg, parameters* ps );

void para_pos_ini( config_t* cfg, parameters* ps )
{
    ps->pos = new double [ps->n_dim];
    para_pos_config( cfg, ps );
    set_para_pos( ps );
}

void para_pos_del( parameters* ps )
{
    delete[] ps->pos;
}

#include <cassert>
#include <libconfig.h>
void para_pos_config( config_t* cfg, parameters* ps )
{
    // good for single run. will be overwritten with multiple calculation
    config_setting_t* setting;
    setting = config_lookup( cfg, "position" );
    assert( config_setting_length( setting ) == ps->n_dim );
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        ps->pos[i_dim] = config_setting_get_float_elem( setting, i_dim );
}

void set_para_pos( parameters* ps )
{
    // seidner: will be set in calc_ptot_seidner with rL[i_phi][i_dir]
    // pullerits: will be set directly with para_esmb_update with random numbers
}
