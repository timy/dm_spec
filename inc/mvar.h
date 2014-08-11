#ifndef MVAR_H_
#define MVAR_H_

void mvar_calc_esmb( struct parameters *ps );
void mvar_calc_grid_seidner( struct parameters *ps );
void mvar_calc_grid( struct parameters *ps );

void para_mvar_ini( struct config_t* cfg, struct parameters *ps );
void para_mvar_del( struct parameters *ps );
void set_para_mvar( struct parameters *ps );

#endif
