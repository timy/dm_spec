// polar_a.cc is the polarization for [t0, t1] (the full range)
#include "para.h"
#include "polar.h"
#include "prop.h"
#include "coord.h"
#include "output.h"
#include "eom.h"
#include "debug.h"
#include <complex>

#define complex std::complex<double>

void ptot_from_dm( complex ***pol, double **rho, parameters *ps, long ig=0 );

void ptot_from_dm_org( complex** pol_M, double* rho, parameters *ps );
void ptot_from_dm_7lv( complex* pol_M, double* rho, parameters *ps );

/////////////////////////////////
#include <exception>
complex*** prepare_pol_array( int dim, parameters *ps )
{
  long n_grid = ps->nt * ps->node->n_mvar;
  complex*** pol = NULL;
  try {
    pol = new complex** [n_grid];
    for (long i_grid = 0; i_grid < n_grid; i_grid ++) {
      pol[i_grid] = new complex* [ps->pols->n_dpl];
      for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
        pol[i_grid][i_dpl] = new complex [ps->n_dim];
    }
  } catch (std::exception& e) {
    error( ps, "%s -> %s", "prepare_pol_array", e.what() );
  }
  return pol;
}

void clean_pol_array( int dim, complex ***pol, parameters *ps )
{
  long n_grid = ps->nt * ps->node->n_mvar;
  for (long i_grid = 0; i_grid < n_grid; i_grid ++) {
    for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++) {
      delete[] pol[i_grid][i_dpl];
    }
    delete[] pol[i_grid];
  }
  delete[] pol;
}


// basic
void calc_ptot( complex ***ptot, double** rho, parameters *ps, int ip, long ig )
{
  para_eom peom = { ip, ps, rho };
  prop( &peom );
  ptot_from_dm( ptot, rho, ps, ig );
}

// single spatial phase
void calc_ptot( complex ***ptot, parameters *ps, long ig )
{
  double **rho = prepare_rho_array( ps );
  // assign spatial phase (lab. frame?)
  for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++) {
    ps->ef[i_pulse]->ph = 0.0;
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
      ps->ef[i_pulse]->ph += ps->ef[i_pulse]->k0
        * ps->ef[i_pulse]->kuvL[i_dim] * ps->pos[i_dim];
  }
  calc_ptot( ptot, rho, ps, 0, ig );
  clean_rho_array( rho, ps );
}

void ptot_from_dm( complex ***pol, double **rho, parameters *ps, long ig )
{
  // pol: nt * n_dpl * n_dim         rho: nt * N_EOM
  // deleted later!
  // complex *pol_M = new complex[ps->n_dim];
  complex** pol_M = new complex* [ps->pols->n_dpl];
  for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
    pol_M[i_dpl] = new complex [ps->n_dim];

  for (long it = 0; it < ps->nt; it ++) {
    ptot_from_dm_org( pol_M, rho[it], ps );
    for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
      coord_from_mol_to_lab( pol_M[i_dpl], pol[ig+it][i_dpl], ps );
  }

  for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
    delete[] pol_M[i_dpl];
  delete[] pol_M;
}

void ptot_from_dm_org( complex** pol_M, double* rho, parameters *ps )
{
  complex* r = new complex [ps->n_dpl]; // TODO: move to outside to reduce allocation

  for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++) {
    int m = ps->dipole->index[i_dpl][0];
    int n = ps->dipole->index[i_dpl][1]; // == (4,5), (6,7), (12,13), (14,15)
    int idx = ps->n_lvl + m * (m-1) + 2 * n; // TODO: this is the idx formula, move to
    r[i_dpl] = complex( rho[idx], rho[idx+1] ); // global scope, and pre-calcualte it
  }

  double** dipole = ps->dipole->dipole;
  for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
    if (ps->pols->bPolForEachDpl == true) {
      for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++)
        pol_M[i_dpl][i_dim] = 2.0 * real( dipole[i_dpl][i_dim] * r[i_dpl] );
    } else {
      complex s = 0.0;
      for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++)
        s += dipole[i_dpl][i_dim] * r[i_dpl];
      pol_M[0][i_dim] = 2.0 * real(s);
    }
  delete[] r;
}
