#ifndef EOM_H_
#define EOM_H_

namespace std { template<typename T> class complex; }

struct para_eom
{
    int iphi;              // index of phi
    struct parameters *ps; // parameters of system
    // std::complex<double> **rho;
    double **rho;
};

int equation_of_motion_full( double t, const double y[], double f[],
                        void *peom );


int equation_of_motion_5lv( double t, const double y[], double f[],
                        void *peom );

int equation_of_motion_7lv( double t, const double y[], double f[],
                        void *peom );

void eom_init_cond( double *y, para_eom *peom );

void eom_post_proc( int it, double *y, para_eom *peom );

// int equation_of_motion_RWA( double t, const double y[], double f[],
//                         void *para );

#endif
