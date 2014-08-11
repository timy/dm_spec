#ifndef ESMB_H_
#define ESMB_H_

void para_esmb_ini( struct config_t* cfg, struct parameters *ps );
void para_esmb_del( struct parameters *ps );

void para_esmb_update( long i_esmb, struct parameters *ps );

#endif
