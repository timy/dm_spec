#include "file.h"
#include "para.h"
#include "output.h"
#include "complex.h"
#include <cstdio>

#define complex std::complex<double>

////////////////////////////////////////////////////////////
// data line I/O functions
// ------------------------------------------------------------
// basic input
void input_pol_dat_line_basic( FILE *file, complex** pol, long index,
                               struct parameters *ps )
{
    double pol_re, pol_im;
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        fscanf( file, "%le %le ", &pol_re, &pol_im );
        pol[index][i_dim] = complex( pol_re, pol_im );
    }
}
void input_pol_dat_line_basic( FILE** file, complex** pol, long index,
                               parameters *ps )
{
    double pol_re, pol_im;
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        fscanf( file[i_dim], "%le %le", &pol_re, &pol_im );
        pol[index][i_dim] = complex( pol_re, pol_im );
    }
}

// basic output
void output_pol_dat_line_basic( FILE* file, complex** pol, long index,
                                    struct parameters *ps )
{
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        fprintf( file, "%le %le ", // no break for seidner
                 real( pol[index][i_dim] ), imag( pol[index][i_dim] ) );
}
void output_pol_dat_line_basic( FILE** file, complex** pol, long index,
                                struct parameters *ps )
{
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        fprintf( file[i_dim], "%le %le\n",
                 real( pol[index][i_dim] ), imag( pol[index][i_dim] ) );
}

// ------------------------------------------------------------
// input line (generic)
void input_pol_dat_line( FILE *file, complex** pol, long index, 
                         parameters *ps )
{   // read from a single file, n_column = 2 * n_phase * n_dim
    input_pol_dat_line_basic( file, pol, index, ps );
}
void input_pol_dat_line( FILE** file, complex** pol, long index,
                         parameters *ps )
{   // read from n_dim files, n_column = 2 
    input_pol_dat_line_basic( file, pol, index, ps );

}
// output line (generic)
void output_pol_dat_line( FILE* file, complex** pol, long index,
                          parameters *ps )
{   // write to a single file, n_column = 2 * n_phase * n_dim 
    output_pol_dat_line_basic( file, pol, index, ps );
    fprintf( file, "\n" );
}
void output_pol_dat_line( FILE** file, complex** pol, long index,
                          parameters *ps )
{   // write to (n_phase * n_dim) files, n_column = 2
    output_pol_dat_line_basic( file, pol, index, ps );
}

// ------------------------------------------------------------
// input line (seidner)
void input_pol_dat_line( FILE *file, complex*** pol, long index, 
                         parameters *ps )
{   // read from a single file, n_column = 2 * n_phase * n_dim
    // double pol_re, pol_im;
    for (int ip = 0; ip < ps->seid->n_phase; ip ++)
        input_pol_dat_line_basic( file, pol[ip], index, ps );
}
void input_pol_dat_line( FILE*** file, complex*** pol, long index,
                         parameters *ps )
{   // read from (n_phase * n_dim) files, n_column = 2 
    for (int ip = 0; ip < ps->seid->n_phase; ip ++)
        input_pol_dat_line_basic( file[ip], pol[ip], index, ps );

}
// output line (seidner)
void output_pol_dat_line( FILE* file, complex*** pol, long index,
                          parameters *ps )
{   // write to a single file, n_column = 2 * n_phase * n_dim 
    for (int ip = 0; ip < ps->seid->n_phase; ip ++)
        output_pol_dat_line_basic( file, pol[ip], index, ps );
    fprintf( file, "\n" );
}
void output_pol_dat_line( FILE*** file, complex*** pol, long index,
                          parameters *ps )
{   // write to (n_phase * n_dim) files, n_column = 2
    for (int ip = 0; ip < ps->seid->n_phase; ip ++)
        output_pol_dat_line_basic( file[ip], pol[ip], index, ps );
}
