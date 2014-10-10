#include "para.h"
#include "repr.h"
#include <cstdio>
#include <cmath>
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
    double* energy = ps->energy->energy;
    int* idxErg = ps->energy->index;
    idxErg[0] = 0;
    energy[0] = 0.0;
    idxErg[1] = 1;
    energy[1] = 0.5 * ( E_sum - sqrt( E_dif_2 + 4.0 * J2 ) );
    idxErg[2] = 2;
    energy[2] = 0.5 * ( E_sum + sqrt( E_dif_2 + 4.0 * J2 ) );
    idxErg[3] = 3;
    energy[3] = E_sum; // here one can add some anharmonicity

    // dipole: defined in the mol. frame
    assert( ps->n_dpl == 4 );
    double** dipole = ps->dipole->dipole;
    int** idxDpl = ps->dipole->index;
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        // mu_10
        idxDpl[0][0] = 1;
        idxDpl[0][1] = 0;
        dipole[0][i_dim] = - s_theta * pdl->mu_A[i_dim] + c_theta * pdl->mu_B[i_dim];
        // mu_20
        idxDpl[1][0] = 2;
        idxDpl[1][1] = 0;
        dipole[1][i_dim] = + c_theta * pdl->mu_A[i_dim] + s_theta * pdl->mu_B[i_dim];
        // mu_31
        idxDpl[2][0] = 3;
        idxDpl[2][1] = 1;
        dipole[2][i_dim] = - s_theta * pdl->mu_B[i_dim] + c_theta * pdl->mu_A[i_dim];
        // mu_32
        idxDpl[3][0] = 3;
        idxDpl[3][1] = 2;
        dipole[3][i_dim] = + c_theta * pdl->mu_B[i_dim] + s_theta * pdl->mu_A[i_dim];
    }

    // globally accessible variables may used by other modules:
    ps->repr->theta = theta;
}

#include <libconfig.h>
void para_repr_dimer_config( config_t *cfg, para_repr_dimer_local *pdl )
{
    config_setting_t *setting_1, *setting_2;
    // EA, EB
    setting_1 = config_lookup( cfg, "repr.dimer.E" );
    int n_E = config_setting_length( setting_1 );
    assert( n_E == 2 );
    pdl->EA = config_setting_get_float_elem( setting_1, 0 );
    pdl->EB = config_setting_get_float_elem( setting_1, 1 );
    pdl->EA *= C_cm2au;
    pdl->EB *= C_cm2au;

    // J
    config_lookup_float( cfg, "repr.dimer.J", &(pdl->J) );
    pdl->J *= C_cm2au;

    // mu_A, mu_B
    setting_1 = config_lookup( cfg, "repr.dimer.mu" );
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
