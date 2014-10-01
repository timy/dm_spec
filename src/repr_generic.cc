#include "para.h"
#include "repr.h"
#include "cdf.h"
#include <cstdio>

void repr_set_generic( parameters *ps, para_repr_generic *pg )
{
    for (int i_lvl = 0; i_lvl < ps->n_lvl; i_lvl ++)
        ps->energy->energy[i_lvl] = pg->E[i_lvl];

    for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            ps->dipole->dipole[i_dpl][i_dim] = pg->mu[i_dpl][i_dim];
}

#include <libconfig.h>
void sscanf_st_energy( char* buf, int i, parameters* ps );
void sscanf_st_dipole( char* buf, int i, parameters* ps );

void para_repr_generic_config( config_t *cfg, parameters* ps )
{
    const char* name = NULL;

    config_lookup_string( cfg, "generic.E", &name );
    cdf_read( name, ps->n_lvl, sscanf_st_energy, ps );

    config_lookup_string( cfg, "generic.mu", &name );
    cdf_read( name, ps->n_dpl, sscanf_st_dipole, ps );
}

void sscanf_st_energy( char* buf, int i, parameters* ps ) {
    int* index = ps->energy->index;
    double* energy = ps->energy->energy;
    int nCol = sscanf( buf, "%d %lf", &(index[i]), &(energy[i]) );
    energy[i] *= C_cm2au;
    if (nCol != 2)
        fprintf( stderr, "CDF warning: suspicous ENERGY data format, nCol=%d\n", nCol );
}

void sscanf_st_dipole( char* buf, int i, parameters* ps ) {
    int** index = ps->dipole->index;
    double** dipole = ps->dipole->dipole;
    int nCol = sscanf( buf, "%d %d %lf %lf %lf", &(index[i][0]), &(index[i][1]),
                       &(dipole[i][0]), &(dipole[i][1]), &(dipole[i][2]) );
    if (nCol != 5)
        fprintf( stderr, "CDF warning: suspicous DIPOLE data format, nCol=%d\n", nCol );
}
