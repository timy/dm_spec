#ifndef FIELD_H_
#define FIELD_H_

namespace std { template<typename T> class complex; }

void para_efield_ini( struct config_t* cfg, struct parameters *ps );
void para_efield_del( struct parameters *ps );
void set_para_efield_lab( struct parameters *ps );
void set_para_efield_mol( struct parameters *ps );

// Only these two functions are needed
double field_real( double t, struct para_ef *f );
void field_real_components( double *ef, double t, struct parameters *ps );


// std::complex<double> field_cmplx( double t, struct para_ef *f );
// std::complex<double> field_cmplx_RWA( double t, struct para_ef *f );

#endif
