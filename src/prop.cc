/* prop ODE with GSL */

#include "complex.h"
#include "para.h"
#include "eom.h"
#include "prop.h"
#include "field.h"
#include "debug.h"
#include "file.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

#define complex std::complex<double>

void prop( para_eom *peom )
{
  parameters *ps = peom->ps;
  double hstart = 1e-12;
  double epsabs = 1e-14;
  double epsrel = 1e-14;

  switch (ps->n_lvl) {
  case 2:
    ps->f_eom = equation_of_motion_run_2;
    break;
  case 3:
    ps->f_eom = equation_of_motion_run_3;
    break;
  case 4:
    ps->f_eom = equation_of_motion_run_4;
    break;
  case 5:
    ps->f_eom = equation_of_motion_run_5;
    break;
  case 6:
    ps->f_eom = equation_of_motion_run_6;
    break;
  default:
    error( ps, "No implementation is found for n_lvl = %d", ps->n_lvl );
  }

  gsl_odeiv2_system sys = { ps->f_eom, NULL, (size_t) ps->n_eom, peom };
  gsl_odeiv2_driver *driver;
  driver = gsl_odeiv2_driver_alloc_y_new( &sys, gsl_odeiv2_step_rk8pd,
                                          hstart, epsabs, epsrel );
  double t = ps->time[0];                         // a.u.

#ifndef USE_MPI
  int file_idx[1] = { peom->iphi };
  open_para_file( para_file::DM, NULL, ps, 1, NULL, file_idx, "w" );
  open_para_file( para_file::EF, NULL, ps, 1, NULL, file_idx, "w" );
#endif

  equation_of_motion_ini( peom );
  equation_of_motion_out( 0, peom );
  // propagation
  for (long it = 1; it < (ps->nt); it ++) {
    double ti = ps->time[it];                 // a.u.
    int status = gsl_odeiv2_driver_apply( driver, &t, ti, peom->y );
    if (status != GSL_SUCCESS) error( ps, "%d", status );
    equation_of_motion_out( it, peom );
  }
  // clean
  equation_of_motion_del( peom );
  gsl_odeiv2_driver_free( driver );

#ifndef USE_MPI
  close_para_file( para_file::DM, ps );
  close_para_file( para_file::EF, ps );
#endif
  return;
}

double** prepare_rho_array( parameters *ps )
{
  // rho: nt * N_EOM
  double **rho = new double*[ps->nt];
  for (long it = 0; it < ps->nt; it ++)
    rho[it] = new double[ps->n_eom];
  return rho;
}

void clean_rho_array( double **rho, parameters *ps )
{
  for (long it = 0; it < ps->nt; it ++)
    delete[] rho[it];
  delete[] rho;
}
