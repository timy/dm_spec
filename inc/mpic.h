#ifndef MPIC_H_
#define MPIC_H_

void para_mpic_ini( struct config_t* cfg, struct parameters *ps, long size, long rank );

void para_mpic_del( struct parameters *ps );

void para_mpic_set( struct parameters *ps, long size, long rank );

#endif
