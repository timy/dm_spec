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

void postproc_collect_mpi_grid( char* cfg_file, int n_node, const char* prefix )
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

void postproc_pol_combine_dipole( complex*** pol_from, complex*** pol_to,
                                  parameters* ps )
{
    // pol_from: nx * n_dpl * n_dim, ps corresponds to this one
    // pol_to:   nx *     1 * n_dim
    long ns = ps->mpic->njob;
    for (int is = 0; is < ns; is ++)
        for (int it = 0; it < ps->nt; it ++) {
            long index = is * (ps->nt) + it;
            for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++) {
                for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                    pol_to[index][0][i_dim] += pol_from[index][i_dpl][i_dim];
                }
            }
        }
}

void postproc_pol_dir_combine_dipole( complex**** pol_from, complex**** pol_to,
                                      int n_dir, parameters* ps )
{
    // pol_from: n_dir * nx * n_dpl * n_dim, ps corresponses to this one
    // pol_to:   n_dir * nx *     1 * n_dim
    for (int i_dir = 0; i_dir < n_dir; i_dir ++)
        postproc_pol_combine_dipole( pol_from[i_dir], pol_to[i_dir], ps );
}
