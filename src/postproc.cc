#include "postproc.h"
#include "para.h"
#include "output.h"
#include <cstdlib>
#include "complex.h"
#include "debug.h"
#define complex std::complex<double>

// the program assumes to read raw data from prefix folder which contains
// a res dir storing all output from MPI computation. After post-process,
// data will be output to current-dir/res, irrelevant to prefix.

void postproc_collect_mpi_grid( int n_node, const char* dirBase )
{
    parameters ps;
    para_ini( &ps, dirBase );
    double* grid = new double [ps.node->n_mvar];

    int size = 0;
    if (ps.mpic->partition == para_mpic::GRID) {
        size = n_node;
    } else if (ps.mpic->partition == para_mpic::ESMB) {
        size = 1;
    }
    for (int rank = 0; rank < size; rank ++) {
        parameters ps1;
        para_ini( &ps1, dirBase, size, rank );
        ps1.esmb->with_old = 0;
        io_grid_read( para_file::GRID_2D, grid, dirBase, &ps1 );
        para_del( &ps1 );
    }
    io_grid_write( para_file::GRID_2D, grid, NULL, &ps );
    delete[] grid;
    para_del( &ps );
}
