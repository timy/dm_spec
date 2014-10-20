// Calculate ppar from ptot with pullerits method 
// The original pullerits methods apply the ensemble average and, in 
// principle, only the signal in the specific direction survives. However,
// the method cannot resolve high-order signals due to the fact that the
// errors multiplied by low-order signals are comparable with the high-order
// signals which destroy the feasibility of the method. Therefore we have 
// improved the method by S = S - S0 and then apply the ensemble average.

// the original method is called "ppar_extract_pullerits_original", which
// only calls "ppar_extract_pullerits_0"
// the improved method is called "ppar_extract_pullerits", which calls both
// "ppar_extract_pullerits_0" and "ppar_extract_pullerits_1" and needs to 
// run twice.

#include "para.h"
#include "polar.h"
#include "file.h"
#include "mpic.h"
#include "output.h"
#include "complex.h"
#include "debug.h"
#include "eom.h"

#define N_NODE   20
#define N_DIR_0  6 //45
#define N_DIR_1  39

#define complex std::complex<double>
#define EYE complex(0.0, 1.0)

int dir_index_0[N_DIR_0][3] = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}//,
    // {-1, 1, 1}, {1, -1, 1}, {1, 1, -1}
};

int dir_index_1[N_DIR_1][3] = {
    {-1,  1,  1},  { 1, -1,  1},  { 1,  1, -1}, // 0,  1,   2
    {-2,  1,  0},  { 1, -2,  0},  {-2,  0,  1}, // 3,  4,   5
    { 0, -2,  1},  { 1,  0, -2},  { 0,  1, -2}, // 6,  7,   8
    
    { 1, -1, -1},  {-1,  1, -1},  {-1, -1,  1}, // 9,  10, 11
    { 2, -1,  0},  {-1,  2,  0},  { 2,  0, -1}, // 12, 13, 14
    { 0,  2, -1},  {-1,  0,  2},  { 0, -1,  2}, // 15, 16, 17
    
    { 3,  0,  0},  { 0,  3,  0},  { 0,  0,  3}, // 18, 19, 20
    { 2,  1,  0},  { 2,  0,  1},  { 1,  2,  0}, // 21, 22, 23
    { 0,  2,  1},  { 1,  0,  2},  { 0,  1,  2}, // 24, 25, 26
    { 1,  1,  1},                               // 27
    
    {-3,  0,  0},  { 0, -3,  0},  { 0,  0, -3}, // 28, 29, 30
    {-2, -1,  0},  {-2,  0, -1},  {-1, -2,  0}, // 31, 32, 33
    { 0, -2, -1},  {-1,  0, -2},  { 0, -1, -2}, // 34, 35, 36
    {-1, -1, -1},  { 0,  0,  0}                 // 37, 38        
};


// int dir_index_0[N_DIR_0][3] = {
//     { 1,  0,  0},  { 0,  1,  0},  { 0,  0,  1}, // 0,  1,   2
//     {-1,  0,  0},  { 0, -1,  0},  { 0,  0, -1}, // 3,  4,   5

//     {-1,  1,  1},  { 1, -1,  1},  { 1,  1, -1}, // 6,  7,   8
//     {-2,  1,  0},  { 1, -2,  0},  {-2,  0,  1}, // 9,  10, 11
//     { 0, -2,  1},  { 1,  0, -2},  { 0,  1, -2}, // 12, 13, 14
    
//     { 1, -1, -1},  {-1,  1, -1},  {-1, -1,  1}, // 15, 16, 17
//     { 2, -1,  0},  {-1,  2,  0},  { 2,  0, -1}, // 18, 19, 20
//     { 0,  2, -1},  {-1,  0,  2},  { 0, -1,  2}, // 21, 22, 23
    
//     { 3,  0,  0},  { 0,  3,  0},  { 0,  0,  3}, // 24, 25, 26
//     { 2,  1,  0},  { 2,  0,  1},  { 1,  2,  0}, // 27, 28, 29
//     { 0,  2,  1},  { 1,  0,  2},  { 0,  1,  2}, // 30, 31, 32
//     { 1,  1,  1},                               // 33
    
//     {-3,  0,  0},  { 0, -3,  0},  { 0,  0, -3}, // 34, 35, 36
//     {-2, -1,  0},  {-2,  0, -1},  {-1, -2,  0}, // 37, 38, 39
//     { 0, -2, -1},  {-1,  0, -2},  { 0, -1, -2}, // 40, 41, 42
//     {-1, -1, -1},  { 0,  0,  0}                 // 43, 44
// };


void ppar_extract_pullerits_original( parameters* ps );
void ppar_extract_pullerits( parameters* ps );

int main( int argc, char* argv[] )
{
    parameters ps;
    ps.f_eom = equation_of_motion_full; // no use
    para_ini( &ps, "../../cfg/parameters.cfg" );

    // ppar_extract_pullerits_original( &ps );
    ppar_extract_pullerits( &ps );
    
    para_del( &ps );
    fprintf( stdout, "Hasta la vista!\n" );
}

void ppar_extract_pullerits_0( complex*** ppar_1d_0, parameters* ps );
void ppar_extract_pullerits_1( complex*** ppar_1d_0, complex*** ppar_1d_1,
                               parameters* ps );

void ppar_extract_pullerits_original( parameters* ps )
{
    complex **ppar_1d_0[N_DIR_0];
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++)
        ppar_1d_0[i_dir] = prepare_pol_array( ps );

    ppar_extract_pullerits_0( ppar_1d_0, ps );
    
    char str_buf[256];
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++) {
        sprintf( str_buf, "res/ppar_1d_0_%d.dat", i_dir );
        FILE* file = fopen( str_buf, "w" );
        for (long it = 0; it < ps->nt; it ++) {
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                fprintf( file, "%le %le ",
                         real( ppar_1d_0[i_dir][it][i_dim] ),
                         imag( ppar_1d_0[i_dir][it][i_dim] ) );
            fprintf( file, "\n" );
        }
        fclose( file );
    }
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++)
        clean_pol_array( ppar_1d_0[i_dir], ps );
}

void ppar_extract_pullerits( parameters* ps )
{
    complex **ppar_1d_0[N_DIR_0];
    complex **ppar_1d_1[N_DIR_1];
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++)
        ppar_1d_0[i_dir] = prepare_pol_array( ps );
    for (int i_dir = 0; i_dir < N_DIR_1; i_dir ++)
        ppar_1d_1[i_dir] = prepare_pol_array( ps );
    
    ppar_extract_pullerits_0( ppar_1d_0, ps );
    ppar_extract_pullerits_1( ppar_1d_0, ppar_1d_1, ps );

    char str_buf[256];
    // output ppar_1d_0
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++) {
        sprintf( str_buf, "res/ppar_1d_0_%d.dat", i_dir );
        FILE* file = fopen( str_buf, "w" );
        for (long it = 0; it < ps->nt; it ++) {
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                fprintf( file, "%le %le ",
                         real(ppar_1d_0[i_dir][it][i_dim]),
                         imag(ppar_1d_0[i_dir][it][i_dim]) );
            fprintf( file, "\n" );
        }
        fclose( file );
    }
    // output ppar_1d_1
    for (int i_dir = 0; i_dir < N_DIR_1; i_dir ++) {
        sprintf( str_buf, "res/ppar_1d_1_%d.dat", i_dir);
        FILE* file = fopen( str_buf, "w" );
        for (long it = 0; it < ps->nt; it ++) {
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                fprintf( file, "%le %le ", 
                         real(ppar_1d_1[i_dir][it][i_dim]),
                         imag(ppar_1d_1[i_dir][it][i_dim]) );
            fprintf( file, "\n" );
        }
        fclose( file );
    }

    for (int i_dir = 0; i_dir < N_DIR_1; i_dir ++)
        clean_pol_array( ppar_1d_1[i_dir], ps );
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++)
        clean_pol_array( ppar_1d_0[i_dir], ps );
}


void read_rL_from_file( int rank, parameters* ps );
void read_ptot_1d_from_file( complex** ptot_1d, int rank, parameters* ps );


void ppar_extract_pullerits_0( complex*** ppar_1d_0, parameters* ps )
{
    complex **ptot_1d = prepare_pol_array( ps );

    complex sum_exp_kr = 0.0;

    // loop over all samples in all files from MPI nodes
    for (int rank = 0; rank < N_NODE; rank ++) {
        para_mpic_set( ps, N_NODE, rank );
        int ns = ps->node->n_esmb;
        fprintf( stdout, "rank = %d: njob = %d\n", rank, ns );

        int file_idx[1] = { rank };
        open_para_file_read( para_file::RL, "../../", ps, 1, file_idx );
        open_para_file_read( para_file::PTOT_1D, "../../", ps, 1, file_idx );

        for (int i_esmb = 0; i_esmb < ns; i_esmb ++) {
            read_rL_from_file( rank, ps );
            read_ptot_1d_from_file( ptot_1d, rank, ps );

            for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++) {
                double phase = 0.0;
                for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
                    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                        phase += dir_index_0[i_dir][i_pulse]
                            * ps->ef[i_pulse]->k0
                            * ps->ef[i_pulse]->kuvL[i_dim]
                            * ps->pos[i_dim];
                complex exp_kr = exp( - EYE * phase );

                if (i_dir==0)
                    sum_exp_kr += exp_kr;

                for (long it = 0; it < ps->nt; it ++)
                    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                        ppar_1d_0[i_dir][it][i_dim] +=
                            exp_kr * ptot_1d[it][i_dim];
            }
        }

        close_para_file( para_file::PTOT_1D, ps );
        close_para_file( para_file::RL, ps );
    }
    
    fprintf( stdout, "sum_expr_kr = (%lf, %lf)\n",
             real( sum_exp_kr ), imag( sum_exp_kr ) );

    // normalization
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++)
        for (long it = 0; it < ps->nt; it ++)
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                ppar_1d_0[i_dir][it][i_dim] /= ps->esmb->n_esmb;

    // find the maximum value
    FILE* file_max = fopen( "res/max_val_0.dat", "w" );
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++) {
        double max_val = 0.0;
        for (long it = 0; it < ps->nt; it ++)
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                double v = fabs( real( ppar_1d_0[i_dir][it][i_dim] ) );
                max_val = max_val > v ? max_val : v;
            }
        fprintf( file_max, "%d %le\n", i_dir, max_val );
    }
    fclose( file_max );
    
    clean_pol_array( ptot_1d, ps );
}

void ppar_extract_pullerits_1( complex*** ppar_1d_0, complex*** ppar_1d_1,
                               parameters* ps )
{
    complex **ptot_1d_0 = prepare_pol_array( ps );
    complex **ptot_1d_1 = prepare_pol_array( ps );
    
    for (int rank = 0; rank < N_NODE; rank ++) {
        para_mpic_set( ps, N_NODE, rank );
        int ns = ps->node->n_esmb;
        fprintf( stdout, "rank = %d: njob = %d\n", rank, ns );
        
        int file_idx[1] = { rank };
        open_para_file_read( para_file::RL, "../../", ps, 1, file_idx );
        open_para_file_read( para_file::PTOT_1D, "../../", ps, 1,
                             file_idx );

        for (int i_esmb = 0; i_esmb < ns; i_esmb ++) {
            read_rL_from_file( rank, ps );
            read_ptot_1d_from_file( ptot_1d_1, rank, ps );

            // reset ptot_1d_0 to 0
            for (long it = 0; it < ps->nt; it ++)
                for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                    ptot_1d_0[it][i_dim] = 0.0;

            // sum of the 0th order
            for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++) {
                // spatial phase
                double phase = 0.0;
                for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
                    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                        phase += dir_index_0[i_dir][i_pulse]
                            * ps->ef[i_pulse]->k0
                            * ps->ef[i_pulse]->kuvL[i_dim]
                            * ps->pos[i_dim];
                complex expr_kr = exp( + EYE * phase );
                // sum ptot_0
                for (long it = 0; it < ps->nt; it ++)
                    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                        ptot_1d_0[it][i_dim] += 
                            expr_kr * ppar_1d_0[i_dir][it][i_dim];
            }

            // substraction: ptot - ptot_0
            for (long it = 0; it < ps->nt; it ++)
                for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                    ptot_1d_1[it][i_dim] -= ptot_1d_0[it][i_dim];

            // if (rank == 0 && i_esmb == 0) {
            //     FILE* file_test = fopen( "res/test.dat", "w" );
            //     for (long it = 0; it < ps->nt; it ++) {
            //         for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            //             fprintf( file_test, "%le %le ", 
            //                      real(ptot_1d_1[it][i_dim]),
            //                      imag(ptot_1d_1[it][i_dim]) );
            //         fprintf( file_test, "\n" );
            //     }
            //     fclose( file_test );
            // }

            // ppar_1d_1
            for (int i_dir = 0; i_dir < N_DIR_1; i_dir ++) {
                // spatial phase
                double phase = 0.0;
                for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
                    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                        phase += dir_index_1[i_dir][i_pulse]
                            * ps->ef[i_pulse]->k0
                            * ps->ef[i_pulse]->kuvL[i_dim]
                            * ps->pos[i_dim];
                complex expr_kr = exp( - EYE * phase );
                // ppar_1
                for (long it = 0; it < ps->nt; it ++)
                    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                        ppar_1d_1[i_dir][it][i_dim] +=
                            expr_kr * ptot_1d_1[it][i_dim];
            }
        }
        close_para_file( para_file::PTOT_1D, ps );
        close_para_file( para_file::RL, ps );
    }
    
    // normalization
    for (int i_dir = 0; i_dir < N_DIR_1; i_dir ++)
        for (long it = 0; it < ps->nt; it ++)
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                ppar_1d_1[i_dir][it][i_dim] /= ps->esmb->n_esmb;
    
    // find the maximum value
    FILE* file_max = fopen( "res/max_val_1.dat", "w" );
    for (int i_dir = 0; i_dir < N_DIR_1; i_dir ++) {
        double max_val = 0.0;
        for (long it = 0; it < ps->nt; it ++)
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                double v = fabs( real( ppar_1d_1[i_dir][it][i_dim] ) );
                max_val = max_val > v ? max_val : v;
            }
        fprintf( file_max, "%d %le\n", i_dir, max_val );
    }
    fclose( file_max );
    
    clean_pol_array( ptot_1d_1, ps );
    clean_pol_array( ptot_1d_0, ps );
}


void read_rL_from_file( int rank, parameters* ps )
{        
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        fscanf( ps->file->one[para_file::RL]->fptr, "%le ",
                &(ps->pos[i_dim]) );
}

void read_ptot_1d_from_file( complex** ptot_1d, int rank, parameters* ps )
{
    // read ptot_1d as
    // Re_x(t1) Re_y(t1) Re_z(t1) Re_x(t2) Re_y(t2) Re_z(t2) ..
    double ptot_1d_var;
    for (long it = 0; it < ps->nt; it ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
            fscanf( ps->file->one[para_file::PTOT_1D]->fptr,
                    "%le ", &ptot_1d_var );
            ptot_1d[it][i_dim] = complex( ptot_1d_var, 0.0 );
        }
}
