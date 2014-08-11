#ifndef COORD_H_
#define COORD_H_

void para_coord_ini( struct config_t* cfg, struct parameters *ps );
void para_coord_del( struct parameters *para );

void set_para_coord( parameters *para );

template <class T>
void coord_from_lab_to_mol( T *v_a, T *v_b, struct parameters *ps );

template <class T>
void coord_from_mol_to_lab( T *v_a, T *v_b, struct parameters *ps );

#endif
