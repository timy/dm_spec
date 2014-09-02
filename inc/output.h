#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <cstdio>

void output_info( para_file::file_type type, struct parameters* ps );

void output_k_and_r( long i_esmb, FILE *file, struct parameters *ps );

void output_mol_orient( FILE* file, struct parameters *ps );

void output_ef_kL( FILE* file, struct parameters *ps );
void input_ef_kL( FILE* file, double** kL, struct parameters *ps );
void output_ef_kuvL( FILE* file, struct parameters *ps );
void output_ef_kuvM( FILE* file, struct parameters *ps );
void output_ef_EuvL( FILE* file, struct parameters *ps );
void output_ef_EuvM( FILE* file, struct parameters *ps );
void output_rL_seidner( FILE* file, struct parameters *ps );
void output_rM_seidner( FILE* file, struct parameters *ps );


////////////////////////////////////////
// input and output grid data
void io_grid_write( para_file::file_type type, double *s,
                    char* prefix, struct parameters *ps );
void io_grid_read( para_file::file_type type, double *s,
                   char* prefix, struct parameters *ps );

////////////////////////////////////////
// input and output polar data
typedef struct _IO_FILE FILE;

// line operation functions
// generic
void input_pol_dat_line( FILE *file, std::complex<double>** pol,
                         long index, struct parameters *ps );
void input_pol_dat_line( FILE** file, std::complex<double>** pol,
                         long index, struct parameters *ps );
void output_pol_dat_line( FILE* file, std::complex<double>** pol,
                          long index, struct parameters *ps );
void output_pol_dat_line( FILE** file, std::complex<double>** pol,
                          long index, struct parameters *ps );
// seidner
void input_pol_dat_line( FILE *file, std::complex<double>*** pol,
                         long index, struct parameters *ps );
void input_pol_dat_line( FILE*** file, std::complex<double>*** pol,
                         long index, struct parameters *ps );
void output_pol_dat_line( FILE* file, std::complex<double>*** pol,
                          long index, struct parameters *ps );
void output_pol_dat_line( FILE*** file, std::complex<double>*** pol,
                          long index, struct parameters *ps );

// high level functions

// 1D
// template <typename U>
// void output_pol_1d( para_file::file_type type, U pol, parameters* ps,
//                     int n_idx, int *idx );

// 2D
template<typename U>
void output_pol_2d( para_file::file_type type, U pol, long ns,
                    parameters* ps, int n_idx, int* idx );

void io_rl_write( struct parameters* ps );
void io_rl_read( struct parameters* ps );

void io_pol_write( FILE** file, std::complex<double>** pol, struct parameters* ps );
void io_pol_read( FILE** file, std::complex<double>** pol, struct parameters* ps );

void io_pol_dir_write( para_file::file_type type, std::complex<double>*** pol_2d,
                       int n_dir, char* prefix, struct parameters *ps );
void io_pol_dir_read( para_file::file_type type, std::complex<double>*** pol_2d,
                      int n_dir, char* prefix, struct parameters *ps );

#endif // OUTPUT_H_
