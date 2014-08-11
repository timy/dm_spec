#include "file.h"
#include "para.h"
#include "output.h"
#include "complex.h"
#include <cstdio>

#define complex std::complex<double>

////////////////////////////////////////////////////////////
// template

// low-level IO: by file pointer
// 1D
template <typename U>
void input_pol_1d_fptr( FILE *file, U pol, parameters *ps )
{
    double t;
    for (long it = 0; it < ps->nt; it ++) {
        long index = it;
        input_pol_dat_line( file, pol, index, ps );
    }
}
template <typename U>
void output_pol_1d_fptr( FILE *file, U pol, parameters *ps )
{
    for (long it = 0; it < ps->nt; it ++) {
        long index = it;
        output_pol_dat_line( file, pol, index, ps );
    }
}
// 2D
template <typename T, typename U>
void output_pol_2d_fptr( T file, U pol, long ns, parameters* ps )
{
    for (long is = 0; is < ns; is ++)
        for (long it = 0; it < ps->nt; it ++) {
            long index = is * (ps->nt) + it;
            output_pol_dat_line( file, pol, index, ps );
        }
}
template <typename T, typename U>
void input_pol_2d_fptr( T file, U pol, long ns, parameters* ps )
{
    for (long is = 0; is < ns; is ++)
        for (long it = 0; it < ps->nt; it ++) {
            long index = is * (ps->nt) + it;
            input_pol_dat_line( file, pol, index, ps );
        }
}

// ------------------------------------------------------------
// high-level IO: by file type
// 1D
template<typename U>
void output_pol_1d( para_file::file_type type, U pol,
                    parameters* ps, int n_idx, int* idx )
{
    open_para_file_write( type, NULL, ps, n_idx, idx );
    output_pol_1d_fptr( ps->file->one[type]->fptr, pol, ps );
    close_para_file( type, ps );
}
// 2D
template<typename U>
void output_pol_2d( para_file::file_type type, U pol, long ns,
                    parameters* ps, int n_idx, int* idx )
{
    open_para_file_write( type, NULL, ps, n_idx, idx );
    output_pol_2d_fptr( ps->file->one[type]->fptr, pol, ns, ps );
    close_para_file( type, ps );
}


////////////////////////////////////////////////////////////
// Instantiation
// -------------------------------------------------------
// generic

// low-level
template void output_pol_1d_fptr<complex**>
( FILE* file, complex** pol, parameters* ps );

template void input_pol_1d_fptr<complex**>
( FILE* file, complex** pol, parameters* ps );

template void output_pol_2d_fptr<FILE**, complex**>
( FILE** file, complex** pol, long ns, parameters* ps );
template void output_pol_2d_fptr<FILE*, complex**>
( FILE* file, complex** pol, long ns, parameters* ps );

template void input_pol_2d_fptr<FILE**, complex**>
( FILE** file, complex** pol, long ns, parameters* ps );
template void input_pol_2d_fptr<FILE*, complex**>
( FILE* file, complex** pol, long ns, parameters* ps );

// high-level
template void output_pol_1d<complex**>
( para_file::file_type type, complex** pol, parameters* ps, 
  int n_idx, int* idx );
template void output_pol_2d<complex**>
( para_file::file_type type, complex** pol, long ns, parameters* ps,
  int n_idx, int* idx );


// -------------------------------------------------------
// seidner

// low-level
template void output_pol_1d_fptr<complex***>
( FILE* file, complex*** pol, parameters* ps );

template void input_pol_1d_fptr<complex***>
( FILE* file, complex*** pol, parameters* ps );

template void output_pol_2d_fptr<FILE***, complex***>
( FILE*** file, complex*** pol, long ns, parameters* ps );
template void output_pol_2d_fptr<FILE*, complex***>
( FILE* file, complex*** pol, long ns, parameters* ps );

template void input_pol_2d_fptr<FILE***, complex***>
( FILE*** file, complex*** pol, long ns, parameters* ps );
template void input_pol_2d_fptr<FILE*, complex***>
( FILE* file, complex*** pol, long ns, parameters* ps );

// high-level
template void output_pol_1d<complex***>
( para_file::file_type type, complex*** pol, parameters* ps, 
  int n_idx, int* idx );
template void output_pol_2d<complex***>
( para_file::file_type type, complex*** pol, long ns, parameters* ps,
  int n_idx, int* idx );


#include "debug.h"

