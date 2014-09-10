#include "debug.h"
#include "mvar.h"
#include "file.h"
#include "esmb.h"
#include "para.h"
#include "seidner.h"
#include "polar.h"
#include "output.h"
#include "complex.h"

//#include <cstdlib>

#define complex std::complex<double>

// ------------------------------------------------------------
// The following 3 functions should be defined by user
void mvar_update( long is, long i_esmb, struct parameters *ps );
void mvar_output_grid( para_file::file_type type, parameters *ps );
// ------------------------------------------------------------

#include <gsl/gsl_rng.h>
// void mvar_calc_esmb( parameters *ps )
// {
//     // 1d array: nt * n_dim
//     complex **ptot = prepare_pol_array( 1, ps );

//     gsl_rng_set( (gsl_rng*) ps->esmb->rng, ps->mpic->rank + 1 );

//     long i_esmb_0 = ps->mpic->idx0;
//     long i_esmb_1 = i_esmb_0 + ps->mpic->njob;

//     // fprintf( stdout, "rank=%-3ld, i_esmb_0=%-6ld, i_esmb_1=%-6ld\n",
//     //          ps->mpic->rank, i_esmb_0, i_esmb_1 );

//     int file_idx[1] = { (int)ps->mpic->rank };
//     open_para_file_write( para_file::RL, NULL, ps, 1, file_idx );
//     open_para_file_write( para_file::ORIENT, NULL, ps, 1, file_idx );
//     open_para_file_write( para_file::PTOT_1D, NULL, ps, 1, file_idx );

//     open_para_file_write( para_file::KL, NULL, ps, 1, file_idx );
//     output_ef_kL( ps->file->one[para_file::KL]->fptr, ps );
//     close_para_file( para_file::KL, ps );

//     for (long i_esmb = i_esmb_0; i_esmb < i_esmb_1; i_esmb ++) {
//         para_esmb_update( i_esmb, ps );
//         // mvar_update( is, i_esmb, ps );
//         calc_ptot( ptot, ps, 0 ); // note ig = 0 for 1D
//         io_rl_write( ps );

//         // each line output ptot_1d as (P=2Re(.), so we dont' put Im part)
//         // Re_x(t1) Re_y(t1) Re_z(t1) Re_x(t2) Re_y(t2) Re_z(t2) ..
//         for (long it = 0; it < ps->nt; it ++)
//             for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
//                 fprintf( ps->file->one[para_file::PTOT_1D]->fptr, "%le ",
//                          real( ptot[it][i_dim] ) );
//         fprintf( ps->file->one[para_file::PTOT_1D]->fptr, "\n" );
//     }
//     close_para_file( para_file::PTOT_1D, ps );
//     close_para_file( para_file::ORIENT, ps );
//     close_para_file( para_file::RL, ps );
//     clean_pol_array( 1, ptot, ps );
// }

#include <stdexcept>
void mvar_calc_grid_seidner( parameters *ps )
{
    long ns = ps->mpic->njob;
    long nt = ps->nt;

    // 2d array: n_phase * (ns * nt) * n_dim
    complex ****ppar_2d = prepare_pol_array_seidner( 2, ps );
    complex ****ptot_2d = prepare_pol_array_seidner( 2, ps );

    // If there are old results for ensemble for the same parameters,
    // one can choose to continue the calculation from old data
    if (ps->esmb->with_old == 1) {
        try {
            io_pol_dir_read( para_file::PPAR_2D, ppar_2d, ps->seid->n_phase, NULL, ps );
        } catch (std::runtime_error& e) {
            error( ps, "%s",  "Cannot open old data file." );
            clean_pol_array_seidner( 2, ppar_2d, ps );
            clean_pol_array_seidner( 2, ptot_2d, ps );
            return;
        }
    }

    // 1d array: n_phase * nt * n_dim
    complex ****ppar_1d = prepare_pol_array_seidner( 1, ps );
    complex ****ptot_1d = prepare_pol_array_seidner( 1, ps );

    gsl_rng_set( (gsl_rng*) ps->esmb->rng, 4 );
    gsl_rng_set( (gsl_rng*) ps->seid->rng, 4 );

    for (long i_esmb = 0; i_esmb < ps->esmb->n_esmb; i_esmb ++) {
        para_esmb_update( i_esmb, ps );
        for (long is = 0; is < ns; is ++) {
            mvar_update( is, i_esmb, ps );
            calc_ptot_seidner( ptot_1d, ps );
            calc_ppar_seidner( ppar_1d, ptot_1d, ps );
            for (int i_dir = 0; i_dir < ps->seid->n_phase; i_dir ++)
                for (long it = 0; it < nt; it ++) {
                    long index = is * nt + it;
                    for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
                        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                            ppar_2d[i_dir][index][i_dpl][i_dim] +=
                                ppar_1d[i_dir][it][i_dpl][i_dim];
                            if (i_esmb == 0)
                                ptot_2d[i_dir][index][i_dpl][i_dim] =
                                    ptot_1d[i_dir][it][i_dpl][i_dim];
                        }
                }
        }
        if (ps->mpic->rank == 0)
            if (i_esmb % 10 == 0)
                fprintf( stdout, "Finished sample number: %ld of %ld\n",
                         i_esmb, ps->esmb->n_esmb );
    }

    clean_pol_array_seidner( 1, ppar_1d, ps );
    clean_pol_array_seidner( 1, ptot_1d, ps );

    mvar_output_grid( para_file::GRID_2D, ps );
    io_pol_dir_write( para_file::PPAR_2D, ppar_2d, ps->seid->n_phase, NULL, ps );
    io_pol_dir_write( para_file::PTOT_2D, ptot_2d, ps->seid->n_phase, NULL, ps );

    clean_pol_array_seidner( 2, ppar_2d, ps );
    clean_pol_array_seidner( 2, ptot_2d, ps );
}

void mvar_calc_grid( parameters *ps )
{
    long ns = ps->mpic->njob;
    long nt = ps->nt;

    gsl_rng_set( (gsl_rng*) ps->esmb->rng, 1 );
    mvar_output_grid( para_file::GRID_2D, ps );

    int file_idx[1] = { (int)ps->mpic->rank };
    open_para_file_write( para_file::RL, NULL, ps, 1, file_idx );
    open_para_file_write( para_file::PTOT_2D, NULL, ps, 1, file_idx );
    // ptot: (ns * nt) * n_dim     ptot_1d: nt * n_dim;
    complex** ptot = prepare_pol_array( 2, ps );
    for (long i_esmb = 0; i_esmb < ps->esmb->n_esmb; i_esmb ++) {
        para_esmb_update( i_esmb, ps );
        for (long is = 0; is < ns; is ++) {
            mvar_update( is, i_esmb, ps );
            calc_ptot( ptot, ps, is * nt ); // note ig = is * nt for 2D
        }
        io_pol_write( ps->file->mul[para_file::PTOT_2D]->fptr, ptot, ps );
        io_rl_write( ps );
        // display progress
        if (ps->mpic->rank == 0)
            if (i_esmb % 100 == 0)
                fprintf( stdout, "Finished sample number: %ld of %ld\n",
                         i_esmb, ps->esmb->n_esmb );
    }
    clean_pol_array( 2, ptot, ps );
    close_para_file( para_file::RL, ps );
    close_para_file( para_file::PTOT_2D, ps );
}

void para_mvar_config( config_t* cfg, parameters* ps );
void para_mvar_set( parameters* ps );
void para_mvar_ini( config_t* cfg, parameters* ps )
{
    ps->mvar = new para_mvar;
    para_mvar_config( cfg, ps );
    para_mvar_set( ps );
}

void para_mvar_del( parameters* ps )
{
    delete ps->mvar;
}

void para_mvar_set( parameters* ps )
{
    if (ps->mvar->ny == 1) {
        ps->mvar->dy = 0.0;
    } else {
        ps->mvar->dy = (ps->mvar->y1 - ps->mvar->y0) / (ps->mvar->ny - 1.0);
    }
}

#include <libconfig.h>
void para_mvar_config( config_t* cfg, parameters* ps )
{
    int ny;
    config_lookup_int( cfg, "mvar.ny", &ny );
    ps->mvar->ny = ny;
    config_lookup_float( cfg, "mvar.y0", &(ps->mvar->y0) );
    config_lookup_float( cfg, "mvar.y1", &(ps->mvar->y1) );
    // may need units conversion!!!
    ps->mvar->y0 *= C_fs2au;
    ps->mvar->y1 *= C_fs2au;
}

void para_mvar_update( parameters* ps )
{

}

///////////////////////////////////////////////////////////////
// User-defined func - coherence time with tau or population time with T

void mvar_update( long is, long i_esmb, parameters *ps )
{
    // the current value of variable
    double y = ps->mvar->y0 + (ps->mpic->idx0 + is) * ps->mvar->dy;

    // coherence time (2D echo)
    // 2nd pulse is not changed, 1st pulse is changed
    ps->ef[0]->tc = ps->ef[1]->tc - y;

    // // population time
    // double tau = ps->ef[1] - ps->ef[0];
    // ps->ef[1]->tc = 0.0 - y;
    // ps->ef[0]->tc = ps->ef[1]->tc - tau;
}

void mvar_output_grid( para_file::file_type type, parameters *ps )
{
    double *s = new double[ps->mpic->njob];
    for (long is = 0; is < ps->mpic->njob; is ++) {
        s[is] = ps->mvar->y0 + (ps->mpic->idx0 + is) * ps->mvar->dy;
        s[is] /= C_fs2au;
    }
    io_grid_write( para_file::GRID_2D, s, NULL, ps );
    delete[] s;
}


