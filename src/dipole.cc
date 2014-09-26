#include "dipole.h"
#include "para.h"

void para_dipole_ini( config_t* cfg, parameters* ps )
{
    ps->dipole = new para_dipole;
    ps->dipole->index = new int* [ps->n_dpl];
    ps->dipole->dipole = new double* [ps->n_dpl];
    for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++) {
        ps->dipole->index[i_dpl] = new int [2];
        ps->dipole->dipole[i_dpl] = new double [ps->n_dim];
    }
}

void para_dipole_del( parameters* ps )
{
    for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++) {
        delete[] ps->dipole->dipole[i_dpl];
        delete[] ps->dipole->index[i_dpl];
    }
    delete[] ps->dipole->dipole;
    delete[] ps->dipole->index;
    delete ps->dipole;
}

void para_dipole_config( config_t* cfg, parameters* ps )
{
}
