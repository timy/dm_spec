////////////////////////////////////////////////////////////
// Calculation of ensemble of molecules for 2D spectroscopy
////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "complex.h"
#include "coord.h"
#include "bath.h"
#include "repr.h"
#include "para.h"
#include "polar.h"
#include "spec.h"
#include "mpi_cfg.h"
#include "output.h"
#include "util.h"

#include <mpi.h>

#define complex std::complex<double>

void set_para( parameters *para );
void set_para_core( parameters *para );


void setup_ensemble( int rank, int i_esmb, parameters *ps );
void output_pol_2d( complex*** pol_2d, const char* name, long nx, long ny,
                    double dx, double dy, double x0, double y0,
                    int rank, long index_begin, parameters *ps );

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

    // assign size of the task: ny
    long index_begin;
    long ny = assign_jobs( rank, size, ps.n_tau, &index_begin );

    // ppar_esmbl, ptot_2d: n_phase * (ny * nx) * n_dim
    complex ***ppar_esmbl = prepare_pol_array_seidner( nx*ny, &ps );
    complex ***ptot_2d = prepare_pol_array_seidner( nx*ny, &ps );

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
    srand( 1 );
    for (long i_esmb = 0; i_esmb < n_esmb; i_esmb ++) {
        
        // FIXME: add ensemble effects, modify mpi.cfg
        setup_ensemble( rank, i_esmb, &ps );
        
        if (rank == 0) {
            output_k_and_r( i_esmb, file_kr, &ps );
            if (i_esmb == 0) {
                display_global_info( stdout, &ps );
            }
        }
        // for a certain set of initial parameters, the partial 2D ppar
        for (long iy = 0; iy < ny; iy ++) {
            // center of the 1st pulse
            ps.ef[0]->tc = - ps.T - (index_begin + iy) * ps.d_tau;
            if (i_esmb == 0) {
                display_node_info( file_info, rank, iy, index_begin, &ps );
            }
            // core computation
            calc_ptot_seidner( ptot_1d, &ps );
            calc_ppar_seidner( ppar_1d, ptot_1d, &ps );
            // display_info( rank, i_esmb, iy, file_info, &ps );
            // sum up to obtain averaged ppar_esmbl
            for (int i_dir = 0; i_dir < ps.seid->n_phase; i_dir ++)
                for (long ix = 0; ix < nx; ix ++)
                    for (int i_dim = 0; i_dim < N_DIM; i_dim ++) {
                        long index = iy * nx + ix;
                        ppar_esmbl[i_dir][index][i_dim] +=
                            ppar_1d[i_dir][ix][i_dim];
                        // test total polarization
                        if (i_esmb == 0) { // i_dir -> i_phase
                            ptot_2d[i_dir][index][i_dim] = 
                                ptot_1d[i_dir][ix][i_dim];
                        }
                    }
        }
    }
    clean_pol_array_seidner( ppar_1d, &ps );
    clean_pol_array_seidner( ptot_1d, &ps );
    fclose( file_info );
    
    if (rank == 0) {
        fclose( file_kr );
    }

    output_pol_2d( ppar_esmbl, "ppar_2d", nx, ny, dx, ps.d_tau,
                   x0, 0.0, rank, index_begin, &ps );
    output_pol_2d( ptot_2d, "ptot_2d", nx, ny, dx, ps.d_tau,
                   x0, 0.0, rank, index_begin, &ps );

    // deallocate 
    clean_pol_array_2d_seidner( nx*ny, ppar_esmbl, nx, ny, &ps );
    clean_pol_array_2d_seidner( nx*ny, ptot_2d, &ps );
    // delete parameters
    del_para( &ps );
    //------------------------------------------------------------
    printf( "Process %d of %d: Hasta la vista!\n", rank, size );
    MPI_Finalize();

    tac( stdout );
    return 0;
}

void output_pol_2d( complex*** pol_2d, const char* name, long nx, long ny,
                    double dx, double dy, double x0, double y0,
                    int rank, long index_begin, parameters *ps )
{
    char file_name[256];
    sprintf( file_name, "raw/%s_%2d.dat", name, rank );
    FILE *file = fopen( file_name, "w" );
    /// tau: 0 -> tau_max, t: 0 -> t_max
    /// order of tau should agree with the above initialization
    for (long iy = 0; iy < ny; iy ++) {
        double y = (y0 + (index_begin + iy) * dy) / C_fs2au;
        for (long ix = 0; ix < nx; ix ++) {
            double x = (x0 + ix * dx) / C_fs2au;
            long index = iy * nx + ix;
            fprintf( file, "%le %le ", y, x );
            for (int ip = 0; ip < (ps->seid->n_phase); ip ++) {
                for (int i_dim = 0; i_dim < N_DIM; i_dim ++)
                    fprintf( file, "%le %le ", 
                             real( pol_2d[ip][index][i_dim] ),
                             imag( pol_2d[ip][index][i_dim] ) );
            }
            fprintf( file, "\n" );
        }
    }
    fclose( file );
}

#include "random.h"

// setup_ensemble is equivalent to set_para
void setup_ensemble( int rank, int i_esmb, parameters *ps )
{
    // FIXME, para_local should be filled completely!!
    parameter_local para_local;
    para_local.EA = 16200.0 * C_cm2au; // + random_normal() * 500.0;
    para_local.EB = 15800.0 * C_cm2au; // + random_normal() * 500.0;
    //para_local.J  = 300.0 * C_cm2au; // + random_normal() * 100.0;
    para_local.J  = 0.0 * C_cm2au; // + random_normal() * 100.0;

    para_local.mu_A[0] = 0.0;
    para_local.mu_A[1] = 0.0;
    //para_local.mu_A[2] = 1.0;
    para_local.mu_A[2] = 1.0;

    para_local.mu_B[0] = 0.0;
    para_local.mu_B[1] = 0.0;
    //para_local.mu_B[2] = -0.23;
    para_local.mu_B[2] = 1.0;

    // Position r
    for (int i_phi = 0; i_phi < (ps->seid->n_phase); i_phi ++)
        for (int i_dim = 0; i_dim < N_DIM; i_dim ++)
            ps->rM[i_phi][i_dim] = (rand() * 1.0 / RAND_MAX - 0.5) * 2e-6;
    
    // Orientation
    double euler_phi   = 2.0 * M_PI * (rand() * 1.0 / RAND_MAX);
    double euler_theta = 1.0 * M_PI * (rand() * 1.0 / RAND_MAX);
    double euler_psi   = 2.0 * M_PI * (rand() * 1.0 / RAND_MAX);
    // double euler_phi   = 0.0;
    // double euler_theta = 0.0;
    // double euler_psi   = 0.0;
    
    // start calculation
    set_para_coord( euler_phi, euler_theta, euler_psi, ps );
    set_para_repr( ps, &para_local );
    set_para_bath( ps );
    set_para_core( ps );
    return;
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
