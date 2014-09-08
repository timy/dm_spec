#ifndef SEIDNER_H_
#define SEIDNER_H_

namespace std { template<typename T> class complex; }

////////////////////////////////////////
// array for polar data

std::complex<double>**** prepare_pol_array_seidner( int dim, struct parameters* ps );
void clean_pol_array_seidner( int dim, std::complex<double>**** pol, struct parameters* ps );

void calc_ptot_seidner( std::complex<double>**** ptot, struct parameters *ps );
void calc_ppar_seidner( std::complex<double>**** ppar, std::complex<double>**** ptot,
                        struct parameters *ps );

void para_seidner_ini( struct config_t* cfg, struct parameters* ps );
void para_seidner_del( struct parameters *ps );
void set_para_seidner( struct parameters *ps );

#endif
