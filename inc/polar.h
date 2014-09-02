#ifndef POLAR_H_
#define POLAR_H_

namespace std { template<typename T> class complex; }

void calc_ptot( std::complex<double> **ptot, double** rho,
                struct parameters *ps, int ip, long ig );

void calc_ptot( std::complex<double> **ptot, struct parameters *ps,
                long ig );

// std::complex<double>** prepare_pol_array( long ng, struct parameters *ps );
// void clean_pol_array( long ng, std::complex<double> **pol,
//                       struct parameters *ps );

std::complex<double>** prepare_pol_array( int dim, struct parameters *ps );
void clean_pol_array( int dim, std::complex<double> **pol,
                      struct parameters *ps );


std::complex<double>** prepare_pol_array( struct parameters *ps );
void clean_pol_array( std::complex<double>** pol, struct parameters *ps );


// template <typename T, typename U>
// void input_pol_2d_fptr( T file, U pol, long ns, struct parameters* ps );
// template <typename T, typename U>
// void output_pol_2d_fptr( T file, U pol, long ns, parameters* ps );

#endif
