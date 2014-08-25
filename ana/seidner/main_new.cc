////////////////////////////////////////////////////////////
// calculate 2D spec from MPI data
////////////////////////////////////////////////////////////
#include "complex.h"
#include "para.h"
#include "output.h"
#include "debug.h"
#include "seidner.h"
#include "postproc.h"
#include <cstdio>

#define complex std::complex<double>

#define N_NODE 20

void collect_mpi_pol( char* cfg_file, int n_node );

int main( int argc, char *argv[] )
{
    collect_mpi_grid( "../../cfg/parameters.cfg", N_NODE, "../../" );
    collect_mpi_pol( "../../cfg/parameters.cfg", N_NODE );
}

void collect_mpi_pol( char* cfg_file, int n_node )
{
    parameters ps = { .f_eom = NULL };
    para_ini( &ps, cfg_file );
    complex*** ppar_2d = prepare_pol_array_seidner( 2, &ps );
    for (int rank = 0; rank < n_node; rank ++) {
        fprintf( stdout, "node %d: processing...\n", rank );
        parameters ps1 = { .f_eom = NULL };
        para_ini( &ps1, cfg_file, n_node, rank );
        ps1.esmb->with_old = 0;
        io_pol_dir_read( para_file::PPAR_2D, ppar_2d, ps1.seid->n_phase,
                         "../../", &ps1 );
        para_del( &ps1 );
    }
    io_pol_dir_write( para_file::PPAR_2D, ppar_2d, ps.seid->n_phase,
                      NULL, &ps );
    clean_pol_array_seidner( 2, ppar_2d, &ps );
    para_del( &ps );
}
