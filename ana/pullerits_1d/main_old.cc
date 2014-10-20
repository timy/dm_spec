// Calculate ppar from ptot with pullerits method 
#include "para.h"
#include "polar.h"
#include "file.h"
#include "mpic.h"
#include "output.h"
#include "complex.h"
#include "debug.h"
#include "eom.h"

#define N_NODE 20
#define N_DIR  3

#define complex std::complex<double>
#define EYE complex(0.0, 1.0)

int dir_index[N_DIR][3] = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}//,
    //    {-1, 1, 1}, {1, -1, 1}, {1, 1, -1}
};

void ppar_extract_pullerits( parameters* ps );

int main( int argc, char* argv[] )
{
    parameters ps;
    ps.f_eom = equation_of_motion_full; // no use
    para_ini( &ps, "../../cfg/parameters.cfg" );

    ppar_extract_pullerits( &ps );

    para_del( &ps );
    fprintf( stdout, "Hasta la vista!\n" );
}

void read_rL_from_file( int rank, parameters* ps );
void read_ptot_1d_from_file( complex** ptot_1d, int rank, parameters* ps );

void ppar_extract_pullerits( parameters* ps )
{    
    complex **ptot_1d = prepare_pol_array( ps );
    complex **ppar_1d[N_DIR];
    for (int i_dir = 0; i_dir < N_DIR; i_dir ++)
        ppar_1d[i_dir] = prepare_pol_array( ps );

    // loop over all samples in all files from MPI nodes
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
            read_ptot_1d_from_file( ptot_1d, rank, ps );
            
            for (int i_dir = 0; i_dir < N_DIR; i_dir ++) {
                double phase = 0.0;
                for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
                    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                        phase += dir_index[i_dir][i_pulse]
                            * ps->ef[i_pulse]->k0
                            * ps->ef[i_pulse]->kuvL[i_dim]
                            * ps->pos[i_dim];
                complex exp_kr = exp( - EYE * phase );
                
                for (long it = 0; it < ps->nt; it ++)
                    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                        ppar_1d[i_dir][it][i_dim] += 
                            exp_kr * ptot_1d[it][i_dim];
            }
        }
        close_para_file( para_file::PTOT_1D, ps );
        close_para_file( para_file::RL, ps );
    }
    
    char str_buf[256];
    for (int i_dir = 0; i_dir < N_DIR; i_dir ++) {
        sprintf( str_buf, "res/ppar_1d_%d.dat", i_dir );
        FILE* file = fopen( str_buf, "w" );
        for (long it = 0; it < ps->nt; it ++) {
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                fprintf( file, "%le %le ",
                         real(ppar_1d[i_dir][it][i_dim]),
                         imag(ppar_1d[i_dir][it][i_dim]) );
            fprintf( file, "\n" );
        }
        fclose( file );
    }
    clean_pol_array( ptot_1d, ps );
    for (int i_dir = 0; i_dir < N_DIR; i_dir ++)
        clean_pol_array( ppar_1d[i_dir], ps );
}

void read_rL_from_file( int rank, parameters* ps )
{        
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        fscanf( ps->file->item[para_file::RL]->fptr, "%le ",
                &(ps->pos[i_dim]) );
        //fprintf( stdout, "%le ", ps->pos[i_dim] );
    }
    //fprintf( stdout, "\n" );
}

void read_ptot_1d_from_file( complex** ptot_1d, int rank, parameters* ps )
{
    // read ptot_1d as
    // Re_x(t1) Re_y(t1) Re_z(t1) Re_x(t2) Re_y(t2) Re_z(t2) ..
    double ptot_1d_var;
    for (long it = 0; it < ps->nt; it ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
            fscanf( ps->file->item[para_file::PTOT_1D]->fptr, "%le ",
                    &ptot_1d_var );
            ptot_1d[it][i_dim] = complex( ptot_1d_var, 0.0 );
        }
}
