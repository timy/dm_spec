#ifndef EOM_H_
#define EOM_H_

namespace std { template<typename T> class complex; }

struct para_eom
{
  int iphi;              // index of phi
  struct parameters *ps; // parameters of system
  // std::complex<double> **rho;
  double **rho;

  // temporary heap object for running eom
  double** Rr;
  double** Ir;
  double** w;
  double** V;
  double*  ef;
  double**** R;
  double*  y;
};

void equation_of_motion_ini( para_eom* peom );
void equation_of_motion_del( para_eom* peom );
void equation_of_motion_out( int it, para_eom* peom );

int equation_of_motion_run_2( double t, const double y[], double* dy, void* peom );
int equation_of_motion_run_3( double t, const double y[], double* dy, void* peom );
int equation_of_motion_run_4( double t, const double y[], double* dy, void* peom );
int equation_of_motion_run_5( double t, const double y[], double* dy, void* peom );
int equation_of_motion_run_6( double t, const double y[], double* dy, void* peom );

#endif
