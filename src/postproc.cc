#include "postproc.h"
#include "para.h"
#include "output.h"
#include <cstdlib>

// the program assumes to read raw data from prefix folder which contains
// a res dir storing all output from MPI computation. After post-process,
// data will be output to current-dir/res, irrelevant to prefix.

void postproc_collect_mpi_grid( char* cfg_file, int n_node, char* prefix )
{
    parameters ps; ps.f_eom = NULL;
    para_ini( &ps, cfg_file );
    double* grid = new double [ps.mpic->njob];
    for (int rank = 0; rank < n_node; rank ++) {
        parameters ps1; ps1.f_eom = NULL;
        para_ini( &ps1, cfg_file, n_node, rank );
        ps1.esmb->with_old = 0;
        io_grid_read( para_file::GRID_2D, grid, prefix, &ps1 );
        para_del( &ps1 );
    }
    io_grid_write( para_file::GRID_2D, grid, NULL, &ps );
    delete[] grid;
    para_del( &ps );
}

// void postproc_
