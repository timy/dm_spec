// Transformations between lab frame and mol frame for a molecule
// oriented by Euler angle (phi, theta, psi)

#include "para.h"
#include "rotation.h"
#include "coord.h"
#include <cmath>

void para_coord_config( config_t* cfg, parameters* ps );
#include <libconfig.h>
void para_coord_ini( config_t* cfg, parameters *ps )
{
    ps->coord = new para_coord;
    ps->coord->mxRotF = new double*[ps->n_dim];
    ps->coord->mxRotB = new double*[ps->n_dim];
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        ps->coord->mxRotF[i_dim] = new double[ps->n_dim];
        ps->coord->mxRotB[i_dim] = new double[ps->n_dim];
    }

    para_coord_config( cfg, ps );
    set_para_coord( ps );
}

void para_coord_del( parameters *ps )
{
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        delete[] ps->coord->mxRotF[i_dim];
        delete[] ps->coord->mxRotB[i_dim];
    }
    delete[] ps->coord->mxRotF;
    delete[] ps->coord->mxRotB;
    delete ps->coord;
}

#include "debug.h"
void para_coord_config( config_t* cfg, parameters* ps )
{
    double phi, theta, psi;
    config_lookup_float( cfg, "euler.phi", &phi );
    config_lookup_float( cfg, "euler.theta", &theta );
    config_lookup_float( cfg, "euler.psi", &psi );
    ps->coord->phi   = phi   * M_PI;
    ps->coord->theta = theta * M_PI;
    ps->coord->psi   = psi   * M_PI;
}

void set_para_coord( parameters *ps )
{
    // ps->coord->phi   = phi;
    // ps->coord->theta = theta;
    // ps->coord->psi   = psi;
    rotation_matrix( ps->coord->phi,
                     ps->coord->theta,
                     ps->coord->psi,
                     ps->coord->mxRotB,
                     ps->coord->mxRotF );
}

template <class T>
void coord_from_lab_to_mol( T *v_a, T *v_b, parameters *ps )
{
    for (int i = 0; i < 3; i ++) {
        v_b[i] = (T) 0;
        for (int j = 0; j < 3; j ++)
            v_b[i] += ps->coord->mxRotB[i][j] * v_a[j];
    }
}

template <class T>
void coord_from_mol_to_lab( T *v_a, T *v_b, parameters *ps )
{
    for (int i = 0; i < 3; i ++) {
        v_b[i] = (T) 0;
        for (int j = 0; j < 3; j ++)
            v_b[i] += ps->coord->mxRotF[i][j] * v_a[j];
    }
}

// instantiation of template functions
template
void coord_from_lab_to_mol<>( double*, double*, parameters* );
template
void coord_from_mol_to_lab<>( double*, double*, parameters* );
#include "complex.h"
#define complex std::complex<double>
template
void coord_from_lab_to_mol<>( complex*, complex*, parameters* );
template
void coord_from_mol_to_lab<>( complex*, complex*, parameters* );
