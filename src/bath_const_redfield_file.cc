#include "para.h"

void bath_set_const_redfield_file( parameters* ps )
{
}

#include "cdf.h"
#include <libconfig.h>
#include <cstdio>

void sscanf_st_const_decay( char* buf, int i, parameters* ps );

void para_bath_const_redfield_file_config( struct config_t* cfg, struct parameters* ps )
{
    int n_tensor;
    const char* name = NULL;
    config_lookup_int( cfg, "bath.const_redfield_file.n_tensor", &n_tensor );
    config_lookup_string( cfg, "bath.const_redfield_file.filename", &name );
    cdf_read( name, n_tensor, sscanf_st_const_decay, ps );
}

#include "complex.h"
#define complex std::complex<double>
void sscanf_st_const_decay( char* buf, int i, parameters* ps ) {
    int m, n, k, l;
    double decay_time; // [fs]
    int nCol = sscanf( buf, "%d %d %d %d %lf", &m, &n, &k, &l, &decay_time );
    if (nCol != 5)
        fprintf( stderr, "CDF warning: suspicious DECAY TIME data format, nCol=%d\n", nCol );

    ps->bath->redfield_tensor[m][n][k][l] = - 1.0 / (decay_time * C_fs2au);
}
