////////////////////////////////////////////////////////////
// calculate 2D spec from MPI data
////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstdio>
#include "complex.h"
#include "para.h"
#include "polar.h"
#include "spec.h"
#include "mpi_cfg.h"
#include "debug.h"

#define complex std::complex<double>

#define N_NODE 20
#define NX 2000
#define NY 38

void set_para( parameters *para, int rank=0, int size=1 );


void read_mpi_pol_2d( complex ***pol_2d,
                      const char* name, struct parameters *ps );

void output_pol_2d( complex ***pol_2d, long nx, double dx, double x0,
                    const char *name, struct parameters *ps );

// void phase_averaging( complex ***ppar_2d, struct parameters *ps );

void process_kr( struct parameters *ps );

int main( int argc, char *argv[] )
{
    const long nx = NX; 
    const long ny = NY;
    int n_node = N_NODE;

    FILE *file;
    char file_name[256];

    parameters ps;
    set_para( &ps, 0, n_node );

    double x0 = ps.t0;
    double dx = ps.dt;
    
    complex ***ppar_2d = prepare_pol_array_2d( nx, ny, &ps );
    complex ***ptot_2d = prepare_pol_array_2d( nx, ny, &ps );

    read_mpi_pol_2d( ppar_2d, "ppar_2d", &ps );

    fprintf( stdout, "Start output 2D partial polarization...\n" );
    output_pol_2d( ppar_2d, nx, dx, x0, "ppar_2d", &ps );

    clean_pol_array_2d( ppar_2d, nx, ny, &ps );
    clean_pol_array_2d( ptot_2d, nx, ny, &ps );

    del_para( &ps );
    fprintf( stdout, "Hasta la vista!\n" );
    return 0;
}


void read_mpi_pol_2d( complex*** pol_2d, const char* name, parameters *ps )
{
    char file_name[256];
    FILE *file;
    long nx = NX;

    for (int i_node = 0; i_node < N_NODE; i_node ++) {

        // assigne size of the task: n_tau
        long index_begin;
        long ny = assign_jobs( i_node, N_NODE, ps->n_tau, &index_begin );

        double x, y, re_val, im_val;
        
        fprintf( stdout, "Reading %s: i_node = %d, ny = %ld\n",
                 name, i_node, ny );

        sprintf( file_name, "../../raw/%s_%2d.dat", name, i_node );
        file = fopen( file_name, "r" );
        if ( file == NULL ) {
            fprintf( stderr, "File %s cannot be opened.\n", file_name );
            exit(-2);
        }
        // note: the order of tau is from tau_max to 0
        for (long iy = 0; iy < ny; iy ++) {
            for (long ix = 0; ix < nx; ix ++) {
                fscanf( file, "%le %le ", &y, &x );
                for (int i_dir = 0; i_dir < (ps->n_phase); i_dir ++) {
                    for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++) {
                        fscanf( file, "%le %le ", &re_val, &im_val );
                        long index = (index_begin + iy) * nx + ix;
                        pol_2d[i_dir][index][i_dim] = 
                            complex( re_val, im_val );
                    }
                }
            }
            fprintf( stdout, "i_node = %d, y = %f\n", i_node, y );
        }
        fclose( file );   
    }
}

void output_pol_2d( complex ***pol_2d, long nx, double dx, double x0,
                    const char *name, parameters *ps )
{
    char file_name[256];
    FILE *file;
    long ny = (ps->n_tau);
    double dy = (ps->d_tau);
    double y0 = 0.0;

    //for (int i_dir = 3; i_dir <= 3; i_dir ++) { // FIXME!!!
    for (int i_dir = 0; i_dir < (ps->n_phase); i_dir ++ ) {        
        fprintf( stdout, "%s@direction: %d\n", name, i_dir );
        //for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++) {
        for (int i_dim = 2; i_dim <= 2; i_dim ++) {
            sprintf( file_name, "res/%s_%2d_%1d.dat", name, i_dir, i_dim );
            file = fopen( file_name, "w" );
            for (long iy = 0; iy < ny; iy ++) {
                for (long ix = 0; ix < nx; ix ++) {
                    //long index = (ny-iy-1) * nx + ix;
                    long index = iy * nx + ix;
                    fprintf( file, "%le %le %le %le\n",
                             (y0 + iy * dy) / C_fs2au,
                             (x0 + ix * dx) / C_fs2au, 
                             real( pol_2d[i_dir][index][i_dim] ),
                             imag( pol_2d[i_dir][index][i_dim] ) );
                }
            }
            fclose( file );
        }
    }
}
