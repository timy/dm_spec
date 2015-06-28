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

void collect_mpi_pol( int n_node, const char* dirBase );

int main( int argc, char *argv[] )
{
  const char dirBase[] = "../../";
  postproc_collect_mpi_grid( N_NODE, dirBase );
  collect_mpi_pol( N_NODE, dirBase );
}

void collect_mpi_pol_grid( int n_node, const char* dirBase, complex**** ppar_2d );
void collect_mpi_pol_esmb( int n_node, const char* dirBase, complex**** ppar_2d );

void collect_mpi_pol( int n_node, const char* dirBase )
{
  parameters ps;
  para_ini( &ps, dirBase );
  complex**** ppar_2d = prepare_pol_array_seidner( 2, &ps );

  if (ps.mpic->partition == para_mpic::GRID) {
    collect_mpi_pol_grid( n_node, dirBase, ppar_2d );
  } else if (ps.mpic->partition == para_mpic::ESMB) {
    collect_mpi_pol_esmb( n_node, dirBase, ppar_2d );
  }

  io_pol_dir_write( para_file::PPAR_2D, ppar_2d, ps.seid->n_phase, NULL, &ps );
  clean_pol_array_seidner( 2, ppar_2d, &ps );
  para_del( &ps );
}

void collect_mpi_pol_grid( int n_node, const char* dirBase, complex**** ppar_2d )
{
  for (int rank = 0; rank < n_node; rank ++) {
    fprintf( stdout, "node %d: processing...\n", rank );
    parameters ps1;
    para_ini( &ps1, dirBase, n_node, rank );
    ps1.esmb->with_old = 0;
    io_pol_dir_read( para_file::PPAR_2D, ppar_2d, ps1.seid->n_phase,
                     dirBase, &ps1 );
    para_del( &ps1 );
  }
}

void collect_mpi_pol_esmb( int n_node, const char* dirBase, complex**** ppar_2d )
{
  for (int rank = 0; rank < n_node; rank ++) {
    fprintf( stdout, "node %d: processing...\n", rank );
    parameters ps1;
    para_ini( &ps1, dirBase, n_node, rank );
    complex**** ppar_2d_tmp = prepare_pol_array_seidner( 2, &ps1 );
    ps1.esmb->with_old = 0;
    io_pol_dir_read( para_file::PPAR_2D, ppar_2d_tmp, ps1.seid->n_phase,
                     dirBase, &ps1 );
    // now sum everything up
    for (int i_dir = 0; i_dir < ps1.seid->n_phase; i_dir ++) {
      for (int is = 0; is < ps1.node->n_mvar; is ++)
        for (int it = 0; it < ps1.nt; it ++) {
          long index = (ps1.node->mvar_0 + is) * ps1.nt + it; // mvar_0 = 0
          for (int i_dpl = 0; i_dpl < ps1.pols->n_dpl; i_dpl ++)
            for (int i_dim = 0; i_dim < ps1.n_dim; i_dim ++) {
              ppar_2d[i_dir][index][i_dpl][i_dim] +=
                ppar_2d_tmp[i_dir][index][i_dpl][i_dim];
            }
        }
    }
    clean_pol_array_seidner( 2, ppar_2d_tmp, &ps1 );
    para_del( &ps1 );
  }
}
