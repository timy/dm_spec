// polar_a.cc is the polarization for [t0, t1] (the full range)
#include "para.h"
#include "polar.h"
#include "prop.h"
#include "coord.h"
#include "output.h"
#include "eom.h"
#include "debug.h"
#include <complex>

#define complex std::complex<double>

void ptot_from_dm( complex **pol, double **rho, parameters *ps,
                   long ig=0 );

void ptot_from_dm_org( complex* pol_M, double* rho, parameters *ps );
void ptot_from_dm_7lv( complex* pol_M, double* rho, parameters *ps );

// complex** prepare_pol_array( long ng, parameters *ps )
// {
//     complex **pol = new complex* [ng];
//     for (long ig = 0; ig < ng; ig ++)
//         pol[ig] = new complex [ps->n_dim];
//     return pol;
// }

// void clean_pol_array( long ng, complex **pol, parameters *ps )
// {
//     for (long ig = 0; ig < ng; ig ++)
//         delete[] pol[ig];
//     delete[] pol;
// }

// complex** prepare_pol_array( parameters *ps )
// {
//     return prepare_pol_array( ps->nt, ps );
// }

// void clean_pol_array( complex **pol, parameters *ps )
// {
//     clean_pol_array( ps->nt, pol, ps );
// }

/////////////////////////////////
complex** prepare_pol_array( int dim, parameters *ps )
{
    long n_grid = ps->nt; // 1d
    if (ps->mvar->dimGrid == 1 && dim == 2)
        n_grid *= (ps->mpic->njob); // 2d

    complex **pol = new complex* [n_grid];
    for (long i_grid = 0; i_grid < n_grid; i_grid ++)
        pol[i_grid] = new complex [ps->n_dim];
    return pol;
}

void clean_pol_array( int dim, complex **pol, parameters *ps )
{
    long n_grid = ps->nt; // 1d
    if (ps->mvar->dimGrid == 1 && dim == 2)
        n_grid *= (ps->mpic->njob); // 2d

    for (long i_grid = 0; i_grid < n_grid; i_grid ++)
        delete[] pol[i_grid];
    delete[] pol;
}


// basic
void calc_ptot( complex **ptot, double** rho, parameters *ps,
                int ip, long ig )
{
    para_eom peom = { ip, ps, rho };
    prop( &peom );
    ptot_from_dm( ptot, rho, ps, ig );
}

// single spatial phase
void calc_ptot( complex **ptot, parameters *ps, long ig )
{
    double **rho = prepare_rho_array( ps );
    // assign spatial phase (lab. frame?)
    for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++) {
        ps->ef[i_pulse]->ph = 0.0;
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            ps->ef[i_pulse]->ph += ps->ef[i_pulse]->k0
                * ps->ef[i_pulse]->kuvL[i_dim] * ps->pos[i_dim];
    }
    calc_ptot( ptot, rho, ps, 0, ig );
    clean_rho_array( rho, ps );
}

void ptot_from_dm( complex **pol, double **rho, parameters *ps, long ig )
{
    // pol: nt * n_dim         rho: nt * N_EOM
    complex *pol_M = new complex[ps->n_dim];
    for (long it = 0; it < ps->nt; it ++) {
        // change here for different energy structure!
        // ptot_from_dm_7lv( pol_M, rho[it], ps );
        ptot_from_dm_org( pol_M, rho[it], ps );

        // comment this for Pullerits' method
        coord_from_mol_to_lab( pol_M, pol[ig+it], ps );

        // for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        //     // pol[ig+it][i_dim] = pol_M[i_dim];
        //     pol[ig+it][i_dim] = complex( 0.0, 0.0 );
        //     for (int j = 0; j < ps->n_dim; j ++)
        //         pol[ig+it][i_dim] += ps->coord->mxRotF[i_dim][j] * pol_M[j];
        // }

        // for Pullerits's method
        // for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        //     pol[ig+it][i_dim] = pol_M[i_dim];
    }
    delete[] pol_M;
}

void ptot_from_dm_org( complex* pol_M, double* rho, parameters *ps )
{
    double mu_10[3], mu_20[3], mu_31[3], mu_32[3];
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        mu_10[i_dim] = ps->dipole[0][i_dim];
        mu_20[i_dim] = ps->dipole[1][i_dim];
        mu_31[i_dim] = ps->dipole[2][i_dim];
        mu_32[i_dim] = ps->dipole[3][i_dim];
    }
    complex r10 = complex( rho[4],  rho[5]  );
    complex r20 = complex( rho[6],  rho[7]  );
    complex r31 = complex( rho[8],  rho[9]  );
    complex r32 = complex( rho[10], rho[11] );
    // complex r12 = complex( rho[12], rho[13] );
    // complex r30 = complex( rho[14], rho[15] );
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        pol_M[i_dim] = 2.0 * real( mu_10[i_dim] * r10 ); //+ mu_20[i_dim] * r20 );
                                   //mu_31[i_dim] * r31 + mu_32[i_dim] * r32 );
    }
}

// should be modified to double* rho, since we can negelct the time index
void ptot_from_dm_7lv( complex* pol_M, double* rho, parameters *ps )
{
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        pol_M[i_dim] = 2.0 * real( ps->dipole[0][i_dim] * complex(rho[7], rho[8]) + ps->dipole[1][i_dim] * complex(rho[9], rho[10]) + ps->dipole[2][i_dim] * complex(rho[11], rho[12]) + ps->dipole[3][i_dim] * complex(rho[13], rho[14]) + ps->dipole[4][i_dim] * complex(rho[15], rho[16]) + ps->dipole[5][i_dim] * complex(rho[17], rho[18]) + ps->dipole[6][i_dim] * complex(rho[19], rho[20]) + ps->dipole[7][i_dim] * complex(rho[21], rho[22]) + ps->dipole[8][i_dim] * complex(rho[23], rho[24]) + ps->dipole[9][i_dim] * complex(rho[25], rho[26]) + ps->dipole[10][i_dim] * complex(rho[27], rho[28]) + ps->dipole[11][i_dim] * complex(rho[29], rho[30]) + ps->dipole[12][i_dim] * complex(rho[31], rho[32]) + ps->dipole[13][i_dim] * complex(rho[33], rho[34]) + ps->dipole[14][i_dim] * complex(rho[35], rho[36]) + ps->dipole[15][i_dim] * complex(rho[37], rho[38]) + ps->dipole[16][i_dim] * complex(rho[39], rho[40]) + ps->dipole[17][i_dim] * complex(rho[41], rho[42]) + ps->dipole[18][i_dim] * complex(rho[43], rho[44]) + ps->dipole[19][i_dim] * complex(rho[45], rho[46]) + ps->dipole[20][i_dim] * complex(rho[47], rho[48]) );
    return;
}
