////////////////////////////////////////////////////////////
// Calculation of ensemble of molecules for 2D spectroscopy
////////////////////////////////////////////////////////////
#include "esmb.h"
#include "complex.h"
#include "coord.h"
#include "bath.h"
#include "repr.h"
#include "para.h"
#include "polar.h"
#include "spec.h"
#include "mpi_cfg.h"
#include "output.h"
#include "file.h"
#include "util.h"

#include <mpi.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#define complex std::complex<double>

void set_para( parameters *para, int rank=0, int size=1 );
void set_para_core( parameters *para );


void setup_ensemble( int rank, int i_esmb, parameters *ps );

void display_node_info( FILE *file, int rank, long iy, long index_begin,
                        parameters *ps );
void display_global_info( FILE *file, parameters *ps );

int main( int argc, char* argv[] )
{
    int rank, size;
    char node_name[1024];
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    printf( "Process %d of %d\n", rank, size );
    //------------------------------------------------------------
    tic();
    // create parameters
    parameters ps;
    set_para( &ps );

    long nx = ps.nt;
    double dx = ps.dt;
    double x0 = ps.t0;

    // read in paramters for MPI
    mpi_configure mpi_cfg;
    read_mpi_cfg( &mpi_cfg );
    long n_esmb = mpi_cfg.n_task_per_cpu;

    // assigne size of the task: ny
    long index_begin;
    long n_job = assign_jobs( rank, size, n_esmb, &index_begin );
    long index_end = index_begin + n_job;

    complex ***ppar_esmbl = prepare_pol_array_seidner( &ps );

    // open files
    char file_name[256];

    sprintf( file_name, "raw/info_%2d.dat", rank );
    FILE *file_info = fopen( file_name, "w" );

    FILE *file_kr = NULL;
    if (rank == 0) {
        file_kr = fopen( "raw/kr.dat", "w" );
    }

    // ppar_1d, ptot_1d: n_phase * nx * n_dim
    complex ***ppar_1d = prepare_pol_array_seidner( &ps );
    complex ***ptot_1d = prepare_pol_array_seidner( &ps );
    // Loop over ensemble. Ensemble parameters are the same for all nodes
    // All nodes share the same random numbers
    srand( rank+1 );
    for (long i_esmb = index_begin; i_esmb < index_end; i_esmb ++) {
        para_esmb_update( i_esmb, &ps );
        // core computation
        calc_ptot( ptot_1d, &ps );
        calc_ppar( ppar_1d, ptot_1d, &ps );
        // display_info( rank, i_esmb, iy, file_info, &ps );
        // sum up to obtain averaged ppar_esmbl
        for (int i_dir = 0; i_dir < ps.n_phase; i_dir ++)
            for (long ix = 0; ix < nx; ix ++)
                for (int i_dim = 0; i_dim < N_DIM; i_dim ++)
                    ppar_esmbl[i_dir][ix][i_dim] +=
                        ppar_1d[i_dir][ix][i_dim];
    }
    clean_pol_array_seidner( ppar_1d, &ps );
    clean_pol_array_seidner( ptot_1d, &ps );
    fclose( file_info );

    if (rank == 0) {
        fclose( file_kr );
    }

    int file_idx[1] = { rank };
    output_pol_1d( para_file::PPAR_1D, ppar_esmbl, &ps, 1, file_idx );

    clean_pol_array_seidner( ppar_esmbl, &ps );
    del_para( &ps );

    //------------------------------------------------------------
    printf( "Process %d of %d: Hasta la vista!\n", rank, size );
    MPI_Finalize();

    tac( stdout );
    return 0;
}

void display_node_info( FILE *file, int rank, long iy, long index_begin,
                        parameters *ps )
{
    fprintf( file, "Info from rank %d:\n", rank );
    fprintf( file, "iy=%ld, tc[0]=%lf, index_begin=%ld\n",
             iy, ps->ef[0]->tc / C_fs2au, index_begin );
    fprintf( file, "-----------------------\n");    
}

void display_global_info( FILE *file, parameters *ps )
{
    fprintf( file, "Global information from rank 0:\n" );
    fprintf( file, "nt=%ld, n_tau=%ld\n", ps->nt, ps->n_tau );    
}
