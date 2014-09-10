#include "para.h"
#include "mvar.h"
#include "debug.h"
#include "util.h"
#include <mpi.h>

#include <cstdio>
#include <cstdlib>
#include "eom.h"

int main( int argc, char* argv[] )
{
    int rank, size;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    // --------------------

    if (rank == 0) tic ();
    // obtain basic parameters
    parameters ps = { .f_eom = equation_of_motion_full };
    para_ini( &ps, "cfg/parameters.cfg", size, rank );

    // actual calculation
    // mvar_calc_esmb( &ps );
    switch (ps.pols->method_ppar) {
    case para_pols::SEIDNER:
        mvar_calc_grid_seidner( &ps );
        break;
    case para_pols::PULLERITS:
        mvar_calc_grid( &ps );
        break;
    }

    // clean parameters
    para_del( &ps );

    // --------------------

    if (rank == 0) {
        tac( stdout );
        fprintf( stdout, "Hasta la vista!\n" );
    }
    MPI_Finalize();
    return 0;
}
