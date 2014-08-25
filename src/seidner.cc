#include "seidner.h"
#include "polar.h"
#include "para.h"
#include "prop.h"
#include "coord.h"
#include "complex.h"
#include <gsl/gsl_rng.h>
#define complex std::complex<double>

void prepare_seidner( parameters *ps );
void para_seidner_config( struct config_t* cfg, struct parameters* ps );

#include <cassert>
#include <libconfig.h>
void para_seidner_ini( config_t* cfg, parameters* ps )
{
    ps->seid = new para_seidner;

    config_setting_t *setting = config_lookup( cfg, "seidner.index" );
    ps->seid->n_phase = config_setting_length( setting );
    assert( ps->seid->n_phase > 0 );

    // rM, rL: n_phase * n_dim
    ps->seid->rL = new double*[ps->seid->n_phase];
    ps->seid->rM = new double*[ps->seid->n_phase];
    for (int i = 0; i < ps->seid->n_phase; i ++) {
        ps->seid->rL[i] = new double[ps->n_dim];
        ps->seid->rM[i] = new double[ps->n_dim];
    }

    // index, phiL, phiM: n_phase * n_pulse
    ps->seid->index = new int*[ps->seid->n_phase];
    ps->seid->phiL = new double*[ps->seid->n_phase];
    ps->seid->phiM = new double*[ps->seid->n_phase];
    for (int i = 0; i < ps->seid->n_phase; i ++) {
        ps->seid->index[i] = new int[ps->n_pulse];
        ps->seid->phiL[i] = new double[ps->n_pulse];
        ps->seid->phiM[i] = new double[ps->n_pulse];
    }

    // pnl_expr: n_phase * n_phase
    ps->seid->pnl_expr = new complex*[ps->seid->n_phase];
    for (int i = 0; i < ps->seid->n_phase; i ++)
        ps->seid->pnl_expr[i] = new complex[ps->seid->n_phase];

    para_seidner_config( cfg, ps );

    ps->seid->rng = (void*) gsl_rng_alloc( gsl_rng_taus2 );
    set_para_seidner( ps );
}

void para_seidner_del( parameters *ps )
{
    gsl_rng_free( (gsl_rng*) ps->seid->rng );
    //delete para->bath;
    // r: n_phase * n_dim
    for (int i = 0; i < ps->seid->n_phase; i ++) {
        delete[] ps->seid->rM[i];
        delete[] ps->seid->rL[i];
    }
    delete[] ps->seid->rM;
    delete[] ps->seid->rL;

    // index, phiL, phiM: n_phase * p_pulse
    for (int i = 0; i < ps->seid->n_phase; i ++) {
        delete[] ps->seid->index[i];
        delete[] ps->seid->phiL[i];
        delete[] ps->seid->phiM[i];
    }
    delete[] ps->seid->index;
    delete[] ps->seid->phiL;
    delete[] ps->seid->phiM;

    // pnl_expr: n_phase * n_phase
    for (int i = 0; i < ps->seid->n_phase; i ++)
        delete[] ps->seid->pnl_expr[i];
    delete[] ps->seid->pnl_expr;

    delete ps->seid;
}

#include <cstdlib>
#include "debug.h"
void para_seidner_config( config_t* cfg, parameters* ps )
{
    config_setting_t *setting;
    // index
    setting = config_lookup( cfg, "seidner.index" );
    for (int i_phase = 0; i_phase < ps->seid->n_phase; i_phase ++) {
        config_setting_t *elem = config_setting_get_elem( setting, i_phase );
        assert( config_setting_length( elem ) == ps->n_pulse );
        for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
            ps->seid->index[i_phase][i_pulse] = config_setting_get_int_elem( elem, i_pulse );
    }
    // rL
    // setting = config_lookup( cfg, "seidner.rL" );
    // if (setting == NULL)
    //     seidner_update_pos( ps );
    // else {
    //     assert( config_setting_length( setting ) == ps->seid->n_phase );
    //     for (int i_phi = 0; i_phi < ps->seid->n_phase; i_phi ++) {
    //         config_setting_t *elem = config_setting_get_elem( setting, i_phi );
    //         assert( config_setting_length( elem ) == ps->n_dim );
    //         for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
    //             config_setting_get_float_elem( elem, i_dim );
    //     }
    // }
}

void set_para_seidner( parameters *ps )
{ // only invoke this after the pulse is set
    prepare_seidner( ps );
}

//////////////////////////////////
complex*** prepare_pol_array_seidner( int dim, parameters* ps )
{
    complex*** pol = new complex** [ps->seid->n_phase];
    for (int ip = 0; ip < ps->seid->n_phase; ip ++)
        pol[ip] = prepare_pol_array( dim, ps );
    return pol;
}
void clean_pol_array_seidner( int dim, complex*** pol, parameters* ps )
{
    for (int ip = 0; ip < ps->seid->n_phase; ip ++)
        clean_pol_array( dim, pol[ip], ps );
    delete[] pol;
}


void calc_ptot_seidner( complex ***ptot, parameters *ps )
{
    double **rho = prepare_rho_array( ps );
    for (int iphi = 0; iphi < ps->seid->n_phase; iphi ++) {
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            ps->pos[i_dim] = ps->seid->rL[iphi][i_dim];
        for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
            ps->ef[i_pulse]->ph = ps->seid->phiL[iphi][i_pulse];
        calc_ptot( ptot[iphi], rho, ps, iphi, 0 );
    }
    clean_rho_array( rho, ps );
}

void calc_ppar_seidner( complex*** ppar, complex*** ptot, parameters *ps )
{
    for (int i_dir = 0; i_dir < ps->seid->n_phase; i_dir ++)
        for (long it = 0; it < ps->nt; it ++)
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                ppar[i_dir][it][i_dim] = complex( 0.0, 0.0 );
                for (int i_phi = 0; i_phi < ps->seid->n_phase; i_phi ++)
                    ppar[i_dir][it][i_dim] +=
                        ps->seid->pnl_expr[i_dir][i_phi] * ptot[i_phi][it][i_dim];
            }
}

#include "debug.h"
extern "C" void zgetrf_( int *M, int *N, complex *A, int *LDA, int *IPIV, int *INFO );
extern "C" void zgetri_( int *N, complex *A, int *LDA, int *IPIV,
                         complex *WORK, int *LWORK, int *INFO );

void seidner_update_pos( parameters *ps )
{
    // rL
    for (int i_phi = 0; i_phi < ps->seid->n_phase; i_phi ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
            double r = gsl_rng_uniform( (gsl_rng*) ps->seid->rng );
            ps->seid->rL[i_phi][i_dim] = (r - 0.5) * 2e-6;
        }

    // rM
    // FIXME - origins can also be specified for r in lab frame
    for (int i = 0; i < ps->seid->n_phase; i ++)
        coord_from_lab_to_mol( ps->seid->rL[i], ps->seid->rM[i], ps );
}

void seidner_update_phase( parameters *ps )
{
    // spatial phase related to different positions of system
    for (int j = 0; j < ps->seid->n_phase; j ++) {
        for (int l = 0; l < ps->n_pulse; l ++) {
            ps->seid->phiL[j][l] = 0.0;
            ps->seid->phiM[j][l] = 0.0;
            for (int k = 0; k < ps->n_dim; k ++) {
                ps->seid->phiL[j][l] += (ps->ef[l]->k0)
                    * (ps->ef[l]->kuvL[k])
                    * (ps->seid->rL[j][k]);
                ps->seid->phiM[j][l] += (ps->ef[l]->k0)
                    * (ps->ef[l]->kuvM[k])
                    * (ps->seid->rM[j][k]);
            }
        }
    }
}

void seidner_update_phase_exp_matrix( complex* PCoef, parameters *ps )
{
    for (int i_dir = 0; i_dir < (ps->seid->n_phase); i_dir ++) {
        for (int i_phi = 0; i_phi < (ps->seid->n_phase); i_phi ++) {
            double Phi = 0.0;
            for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
                Phi += (ps->seid->index[i_dir][i_pulse]) * (ps->seid->phiL[i_phi][i_pulse]);
            PCoef[i_dir*(ps->seid->n_phase)+i_phi] = exp( complex(0.0, Phi) );
        }
    }
 }

void prepare_seidner( parameters *ps )
{
    //seidner_indice( ps );
    seidner_update_pos( ps );
    seidner_update_phase( ps );

    int lwork = ps->seid->n_phase * ps->seid->n_phase;
    complex *PCoef = new complex[lwork];
    seidner_update_phase_exp_matrix( PCoef, ps );

    // ========================================================
    // calculate pnl_expr from the inverse matrix of PCoef
    int *ipiv = new int[ps->seid->n_phase];
    int ierr;

    // LU factorization of a matrix A using partial pivoting with row interchanges
    zgetrf_( &(ps->seid->n_phase), &(ps->seid->n_phase), PCoef, &(ps->seid->n_phase),
             ipiv, &ierr );
    if (ierr != 0) error( ierr );

    complex *work = new complex[lwork];

    // compute the inverse of a matrix using the LU factorization computed by zgetrf_
    zgetri_( &(ps->seid->n_phase), PCoef, &(ps->seid->n_phase), ipiv, work, &lwork, &ierr );
    if (ierr != 0) error( ierr );
    delete[] work;
    delete[] ipiv;

    for (int i = 0; i < ps->seid->n_phase; i ++)
        for (int j = 0; j < ps->seid->n_phase; j ++)
            ps->seid->pnl_expr[j][i] = PCoef[i*(ps->seid->n_phase)+j];
    delete[] PCoef;
}













// information printing ----------------------------------------
void print_pnl_expr( parameters *ps )
{
    FILE *file = fopen( "res/pnl_coef.dat", "w" );
    for (int i_dir = 0; i_dir < ps->seid->n_phase; i_dir ++) {
        for (int i_phi = 0; i_phi < ps->seid->n_phase; i_phi ++) {
            fprintf( file, "%s_%2d %s_%2d:\t%le\t%le\n",
                     "l", i_dir, "phi", i_phi,
                     real( ps->seid->pnl_expr[i_dir][i_phi] ),
                     imag( ps->seid->pnl_expr[i_dir][i_phi] ) );
        }
        fprintf( file, "\n" );
    }
    fclose( file );
}

////////////////////////////////////////////////////////////
// positions (seidner)
void output_rL_seidner( FILE* file, parameters *ps )
{
    for (int i_phi = 0; i_phi < ps->seid->n_phase; i_phi ++) {
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fprintf( file, "%le ", ps->seid->rL[i_phi][i_dim] );
        fprintf( file, "\n" );
    }
    fprintf( file, "\n" );
}

void output_rM_seidner( FILE* file, parameters *ps )
{
    for (int i_phi = 0; i_phi < ps->seid->n_phase; i_phi ++) {
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fprintf( file, "%le ", ps->seid->rM[i_phi][i_dim] );
        fprintf( file, "\n" );
    }
    fprintf( file, "\n" );
}
