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
#include "output.h"

#define complex std::complex<double>

#define N_NODE 20
//#define NX 1400
#define NX 7000

void set_para( parameters *para );

void read_mpi_pol_1d( complex ***pol_1d, FILE *file, 
                      struct parameters *ps );

void process_kr( struct parameters *ps );

int main( int argc, char *argv[] )
{
    // FIXME!
    const long nx = NX; 

    parameters ps;
    set_para( &ps );

    double x0 = ps.t0;
    double dx = ps.dt;

    process_kr( &ps );

    char file_name[256];

    complex ***ppar_esmb = prepare_pol_array_seidner( &ps );
    for (int i_dir = 0; i_dir < ps.n_phase; i_dir ++)
        for (long ix = 0; ix < nx; ix ++)
            for (int i_dim = 0; i_dim < ps.n_dim; i_dim ++)
                ppar_esmb[i_dir][ix][i_dim] = 0.0;
    complex ***ppar_1d = prepare_pol_array_seidner( &ps );

    for (int i_node = 0; i_node < N_NODE; i_node ++) {
    //for (int i_node = 0; i_node < 1; i_node ++) {
        sprintf( file_name, "../../raw/ppar_%2d.dat", i_node );
        FILE *file_ppar_node = fopen( file_name, "r" );
        if (!file_ppar_node) {
            fprintf( stderr, "Cannot open %s!\n", file_name ); exit(-1);
        }
            
        // read data from individual file
        read_mpi_pol_1d( ppar_1d, file_ppar_node, &ps );
        // ensemble averaging over all nodes
        for (int i_dir = 0; i_dir < ps.n_phase; i_dir ++)
            for (long ix = 0; ix < nx; ix ++)
                for (int i_dim = 0; i_dim < ps.n_dim; i_dim ++)
                    ppar_esmb[i_dir][ix][i_dim] +=
                        ppar_1d[i_dir][ix][i_dim];
        fclose( file_ppar_node );
    }
    // output
    FILE *file_ppar_esmb = fopen( "res/ppar_1d.dat", "w" );
    // the following function has been removed!!
    output_pol_1d( ppar_esmb, file_ppar_esmb, &ps );
    fclose( file_ppar_esmb );

    // spec
    spec_1d( ppar_esmb, "res/spec_1d", &ps );


    // clean
    clean_pol_array_seidner( ppar_1d, &ps );
    clean_pol_array_seidner( ppar_esmb, &ps );


    del_para( &ps );
    fprintf( stdout, "Hasta la vista!\n" );
    return 0;
}


void read_mpi_pol_1d( complex*** pol_1d, FILE *file, parameters *ps )
{
    double re, im;
    // refer to output.cc: output_pol_1d(...)
    for (long ix = 0; ix < (ps->nt); ix ++) {
        // fprintf (stdout, "here:%x\n", file);
        fscanf( file, "%le", &re ); // skip the time
        for (int ip = 0; ip < (ps->n_phase); ip ++) {
            for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++) {
                // fprintf( stdout, "ix=%ld, ip=%d, i_dim=%d\n",
                //          ix, ip, i_dim );
                fscanf( file, "%le", &re );
                fscanf( file, "%le", &im );
                pol_1d[ip][ix][i_dim] = complex( re, im );
            }
        }
    }    
}

void process_kr( parameters *ps )
{
    // get n_esmb
    // mpi_configure mpi_cfg;
    // read_mpi_cfg( &mpi_cfg );
    // long n_esmb = mpi_cfg.n_task_per_cpu;
    long n_esmb = 100000;

    char file_name[256];

    // allocation
    double **k = (double**) malloc( (ps->n_pulse) * sizeof(double*) );
    for (int i_pulse = 0; i_pulse < (ps->n_pulse); i_pulse ++)
        k[i_pulse] = (double*) malloc( (ps->n_dim) * sizeof(double) );

    double **r = (double**) malloc( (ps->n_phase) * sizeof(double*) );
    for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++)
        r[i_phi] = (double*) malloc( (ps->n_dim) * sizeof(double) );

    double **phi = (double**) malloc( (ps->n_phase) * sizeof(double*) );
    for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++)
        phi[i_phi] = (double*) malloc( (ps->n_pulse) * sizeof(double) );

    fprintf( stdout, "Reading k & r...\n" );

    // input file
    FILE *file_input = fopen( "../../raw/kr.dat", "r" );
    // output file
    FILE *file_euler = fopen( "res/euler.dat", "w" );
    FILE *file_position = fopen( "res/position.dat", "w" );
    // read wave vector
    for (int i_pulse = 0; i_pulse < (ps->n_pulse); i_pulse ++)
        for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++)
            fscanf( file_input, "%le ", &(k[i_pulse][i_dim]) );

    for (long i_esmb = 0; i_esmb < n_esmb; i_esmb ++) {
        double euler_phi, euler_theta, euler_psi;
        // read and output euler angle
        fscanf( file_input, "%le %le %le\n",
                &euler_phi, &euler_theta, &euler_psi );
        fprintf( file_euler, "%le %le %le\n",
                 euler_phi, euler_theta, euler_psi );

        // read and output spatial points
        for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++) {
            for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++)
                fscanf( file_input, "%le ", &(r[i_phi][i_dim]) );
        }
        for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++) {
            for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++)
                fprintf( file_position, "%le ", r[i_phi][i_dim] );
            fprintf( file_position, "\n" );
        }

        // // test output
        // for (int i_pulse = 0; i_pulse < (ps->n_pulse); i_pulse ++)
        //     for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++)
        //         fprintf( stdout, "%le ", k[i_pulse][i_dim] );
        // fprintf( stdout, "\n" );
        // for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++) {
        //     for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++)
        //         fprintf( stdout, "%le ", r[i_phi][i_dim] );
        //     fprintf( stdout, "\n" );
        // }

        // calculate spatial phase
        for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++)
            for (int i_pulse = 0; i_pulse < (ps->n_pulse); i_pulse ++) {
                phi[i_phi][i_pulse] = 0.0;
                for (int i_dim = 0; i_dim < (ps->n_dim); i_dim ++) {
                    phi[i_phi][i_pulse] += 
                        k[i_pulse][i_dim] * r[i_phi][i_dim];
                }
            }
        
        // // test output
        // fprintf( stdout, "Output spatial phase:\n" );
        // for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++) {
        //     for (int i_pulse = 0; i_pulse < (ps->n_pulse); i_pulse ++)
        //         fprintf( stdout, "%le ", phi[i_phi][i_pulse] );
        //     fprintf( stdout, "\n" );
        // }
    }

    // close files
    fclose( file_input );
    fclose( file_euler );
    fclose( file_position );

    // free
    for (int i_pulse = 0; i_pulse < (ps->n_pulse); i_pulse ++)
        free( k[i_pulse] );
    free( k );

    for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++)
        free( r[i_phi] );
    free( r );

    for (int i_phi = 0; i_phi < (ps->n_phase); i_phi ++)
        free( phi[i_phi] );
    free( phi );

}
