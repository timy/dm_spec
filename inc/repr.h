#ifndef REPR_H_
#define REPR_H_

struct para_repr_dimer_local
{
  double EA, EB;
  double J;
  double mu_A[3], mu_B[3];
};

struct para_repr_generic
{
  double* E;
  double** mu;
};

void para_repr_ini( struct config_t* cfg, struct parameters *ps );
void para_repr_del( struct parameters *ps );
void set_para_repr( struct parameters *ps );

#endif
