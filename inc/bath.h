#ifndef BATH_H_
#define BATH_H_

void para_bath_ini( struct config_t* cfg, struct parameters *ps );
void para_bath_del( struct parameters *ps );
void set_para_bath( struct parameters *ps );

double distribution_boson_n( double w, parameters *ps );
double spectral_density_J( double w, struct parameters *ps );

#endif
