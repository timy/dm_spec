#include "para.h"
#include "repr.h"
#include <cmath>
#include <cstdio>
#include <cassert>

void repr_set_exciton_dimer( parameters *ps, para_repr_dimer_local *pdl )
{
    // some useful values
    double E_sum = (pdl->EA) + (pdl->EB);
    double E_dif = (pdl->EA) - (pdl->EB);
    double E_dif_2 = E_dif * E_dif;
    double J2 = (pdl->J) * (pdl->J);

    // mixture angle
    double theta = 0.5 * atan2( 2.0*(pdl->J), E_dif );
    double s_theta = sin( theta );
    double c_theta = cos( theta );

    // energy
    assert( ps->n_lvl == 4 );
    ps->energy[0] = 0.0;
    ps->energy[1] = 0.5 * ( E_sum - sqrt( E_dif_2 + 4.0 * J2 ) );
    ps->energy[2] = 0.5 * ( E_sum + sqrt( E_dif_2 + 4.0 * J2 ) );
    ps->energy[3] = E_sum;

    // dipole: defined in the mol. frame
    assert( ps->n_dpl == 4 );
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        // mu_10
        ps->dipole[0][i_dim] = - s_theta * pdl->mu_A[i_dim] + c_theta * pdl->mu_B[i_dim];
        // mu_20
        ps->dipole[1][i_dim] = + c_theta * pdl->mu_A[i_dim] + s_theta * pdl->mu_B[i_dim];
        // mu_31
        ps->dipole[2][i_dim] = - s_theta * pdl->mu_B[i_dim] + c_theta * pdl->mu_A[i_dim];
        // mu_32
        ps->dipole[3][i_dim] = + c_theta * pdl->mu_B[i_dim] + s_theta * pdl->mu_A[i_dim];
    }

    // globally accessible variables may used by other modules:
    ps->repr->theta = theta;
}

void repr_set_generic( parameters *ps, para_repr_generic *pg )
{
    // energy
    for (int i_lvl = 0; i_lvl < ps->n_lvl; i_lvl ++)
        ps->energy[i_lvl] = pg->E[i_lvl];

    // dipole momentum
    for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            ps->dipole[i_dpl][i_dim] = pg->mu[i_dpl][i_dim];
}

#include <libconfig.h>
void para_repr_dimer_config( config_t *cfg, para_repr_dimer_local *pdl );
void para_repr_generic_config( config_t *cfg, para_repr_generic *pg );

#include <cstring>
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

    if (count == 1) {
        if (strcmp( type[idx], "dimer" ) == 0) {
            para_repr_dimer_local pdl;
            para_repr_dimer_config( cfg, &pdl );
            repr_set_exciton_dimer( ps, &pdl );
        }
        else if (strcmp( type[idx], "generic" ) == 0) {
            para_repr_generic pg;
            para_repr_generic_config( cfg, &pg );
            repr_set_generic( ps, &pg );
        }
    }
    else
        error( ps, "%d", count );
}

void para_repr_del( parameters *para )
{
    delete para->repr;
}

void para_repr_generic_config( config_t *cfg, para_repr_generic *pg )
{
    // FIXME: to be implemented
}

void para_repr_dimer_config( config_t *cfg, para_repr_dimer_local *pdl )
{
    config_setting_t *setting_1, *setting_2;
    // EA, EB
    setting_1 = config_lookup( cfg, "dimer.E" );
    int n_E = config_setting_length( setting_1 );
    assert( n_E == 2 );
    pdl->EA = config_setting_get_float_elem( setting_1, 0 );
    pdl->EB = config_setting_get_float_elem( setting_1, 1 );
    pdl->EA *= C_cm2au;
    pdl->EB *= C_cm2au;

    // J
    config_lookup_float( cfg, "dimer.J", &(pdl->J) );
    pdl->J *= C_cm2au;

    // mu_A, mu_B
    setting_1 = config_lookup( cfg, "dimer.mu" );
    int n_mu = config_setting_length( setting_1 );
    assert( n_mu == 2 );
    setting_2 = config_setting_get_elem( setting_1, 0 );

    assert( config_setting_length( setting_2 ) == 3 );
    for (int i_dim = 0; i_dim < 3; i_dim ++)
        pdl->mu_A[i_dim] = config_setting_get_float_elem( setting_2, i_dim );
    setting_2 = config_setting_get_elem( setting_1, 1 );

    assert( config_setting_length( setting_2 ) == 3 );
    for (int i_dim = 0; i_dim < 3; i_dim ++)
        pdl->mu_B[i_dim] = config_setting_get_float_elem( setting_2, i_dim );
}
