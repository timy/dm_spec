#ifndef REPR_H_
#define REPR_H_

// FIXME: generalized structure for different sys.
//  struct parameter_local
// {
//     // specific for exciton repr.
//     double EA, EB;
//     double J;
//     double mu_A[N_DIM], mu_B[N_DIM];

//     // generic structure
//     double E[N_LVL];
//     double mu[N_DPL][N_DIM];
//     double position[N_DIM];
// };

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

void para_repr_del( struct parameters *para );

void repr_set_exciton_dimer( struct parameters *ps,
                             struct para_repr_dimer_local *pdl );

void repr_set_generic( struct parameters *ps,
                       struct para_repr_generic *pg );

#endif
