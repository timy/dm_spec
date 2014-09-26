#include "energy.h"
#include "para.h"

void para_energy_ini( config_t* cfg, parameters* ps )
{
    ps->energy = new para_energy;
    ps->energy->index = new int [ps->n_lvl];
    ps->energy->energy = new double [ps->n_lvl];
}

void para_energy_del( parameters* ps )
{
    delete[] ps->energy->energy;
    delete[] ps->energy->index;
    delete ps->energy;
}

void para_energy_config( config_t* cfg, parameters* ps )
{
}
