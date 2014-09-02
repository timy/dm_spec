#ifndef RK4_H_
#define RK4_H_

namespace std { template<typename T> class complex; }

// void prop( std::complex<double> **rho, int iphi, parameters *ps );
void prop( struct para_eom *peom );

double** prepare_rho_array( struct parameters *ps );
void clean_rho_array( double **rho, struct parameters *ps );

#endif
