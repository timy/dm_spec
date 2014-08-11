#ifndef TIMESTEP_H_
#define TIMESTEP_H_

void para_time_ini( struct config_t* cfg, struct parameters* ps );
void para_time_del( struct parameters* ps );
void para_time_write( struct parameters* ps );

#endif
