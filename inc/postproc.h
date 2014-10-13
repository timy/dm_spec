#ifndef POSTPROC_H_
#define POSTPROC_H_

namespace std { template<typename T> class complex; }

void postproc_collect_mpi_grid( int n_node, const char* dirBase );

void postproc_pol_combine_dipole( std::complex<double>*** pol_from,
                                  std::complex<double>*** pol_to,
                                  struct parameters* ps );
void postproc_pol_dir_combine_dipole( std::complex<double>**** pol_from,
                                      std::complex<double>**** pol_to,
                                      int n_dir, struct parameters* ps );
#endif
