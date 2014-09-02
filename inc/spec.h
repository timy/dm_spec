#ifndef SPEC_H_
#define SPEC_H_

namespace std { template<typename T> class complex; }

void spec_1d( std::complex<double> ***ppar, const char *name,
              struct parameters *ps );
void spec_2d( std::complex<double> ***ppar, struct parameters *ps, int id );

typedef struct _IO_FILE FILE;

void spec_1d_test( std::complex<double> *pol, double dx, long nx, FILE* file, parameters *ps );

#endif
