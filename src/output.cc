#include "file.h"
#include "para.h"
#include "field.h"
#include "debug.h"
#include <complex.h>
#include <cstdio>

#define complex std::complex<double>
#define EYE complex( 0.0, 1.0 )

void output_redfield_tensor( FILE* file, parameters *ps )
{
  for (int m = 0; m < ps->n_lvl; m ++) {
    for (int n = 0; n < ps->n_lvl; n ++) {
      for (int k = 0; k < ps->n_lvl; k ++) {
        for (int l = 0; l < ps->n_lvl; l ++) {
          fprintf( file, "R(%d %d <- %d %d): %e + i %e\n", m, n, k, l,
                   real(ps->bath->redfield_tensor[m][n][k][l]),
                   imag(ps->bath->redfield_tensor[m][n][k][l]) );
        }
      }
    }
  }
}

void output_mol_orient( FILE* file, parameters *ps )
{
  double phi = ps->coord->phi;
  double theta = ps->coord->theta;
  double psi = ps->coord->psi;
  // double x = sin(theta) * sin(psi);
  // double y = sin(theta) * cos(psi);
  // double z = cos(theta);

  // equivalent to mxRotF.{0,0,1}, rotation of the z-axis
  double x = sin(theta) * sin(phi);
  double y = -cos(phi) * sin(theta);
  double z = cos(theta);

  fprintf( file, "%le %le %le %le %le %le\n",
           x, y, z, phi, theta, psi );
}

void output_ef_kL( FILE* file, parameters *ps )
{
  for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++) {
    double k0 = ps->ef[i_pulse]->k0;
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
      fprintf( file, "%le ", k0 * ps->ef[i_pulse]->kuvL[i_dim] );
  }
}

void output_ef_kuvL( FILE* file, parameters *ps )
{
  for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
      fprintf( file, "%le ", (ps->ef[i_pulse]->kuvL[i_dim]) );
  fprintf( file, "\n" );
}

void output_ef_kuvM( FILE* file, parameters *ps )
{
  for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
      fprintf( file, "%le ", (ps->ef[i_pulse]->kuvM[i_dim]) );
  fprintf( file, "\n" );
}

void output_ef_EuvL( FILE* file, parameters *ps )
{
  for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
      fprintf( file, "%le ", (ps->ef[i_pulse]->EuvL[i_dim]) );
  fprintf( file, "\n" );
}

void output_ef_EuvM( FILE* file, parameters *ps )
{
  for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
      fprintf( file, "%le ", (ps->ef[i_pulse]->EuvM[i_dim]) );
  fprintf( file, "\n" );
}


// output general information
// used for para_init_config_write and para_init_info_write
#define print_info( file, x ) {                         \
    fprintf( file, "%s,L%d: ", __FILE__, __LINE__ );    \
    dm_fprintf( file, #x );                             \
    fprintf( file, " = " );                             \
    dm_fprintf( file, x );                              \
    fputc( '\n', file );                                \
  }
#define display_section( file, x ) {            \
    fprintf( file, "\n%s\n", x ); }

void dm_fprintf( FILE* file, int d )
{
  fprintf( file, "%d", d );
}
void dm_fprintf( FILE* file, long d )
{
  fprintf( file, "%ld", d );
}
void dm_fprintf( FILE* file, double d )
{
  fprintf( file, "%le", d );
}
void dm_fprintf( FILE* file, const char* d )
{
  fprintf( file, "%s", d );
}

void output_info( para_file::file_type type, parameters* ps )
{
  open_para_file( type, NULL, ps, 0, NULL, NULL, "w" );
  FILE* file = ps->file->item[type]->f[0]->fptr;

  display_section( file, "basic" );
  print_info( file, ps->n_dim );
  print_info( file, ps->n_lvl );
  print_info( file, ps->n_dpl );
  print_info( file, ps->n_pulse );

  display_section( file, "time" );
  print_info( file, ps->nt );
  print_info( file, ps->t0/C_fs2au );
  print_info( file, ps->t1/C_fs2au );
  print_info( file, ps->dt/C_fs2au );
  print_info( file, ps->it0 );
  print_info( file, ps->time[ps->it0]/C_fs2au );

  display_section( file, "core" );
  for (int i = 0; i < ps->n_lvl; i ++)
    print_info( file, ps->energy->energy[i]/C_cm2au );
  for (int i = 0; i < ps->n_dpl; i ++)
    for (int j = 0; j < ps->n_dim; j ++)
      print_info( file, ps->dipole->dipole[i][j] );
  for (int i = 0; i < ps->n_dim; i ++)
    print_info( file, ps->pos[i] );

  display_section( file, "coord" );
  print_info( file, ps->coord->phi/M_PI );
  print_info( file, ps->coord->theta/M_PI );
  print_info( file, ps->coord->psi/M_PI );

  display_section( file, "mvar" );
  print_info( file, ps->mvar->ny );
  print_info( file, ps->mvar->y0 );
  print_info( file, ps->mvar->y1 );
  print_info( file, ps->mvar->dy );
  print_info( file, ps->esmb->n_esmb );

  display_section( file, "mpic" );
  print_info( file, ps->mpic->njob );
  print_info( file, ps->mpic->idx0 );

  display_section( file, "efield" );
  for (int i = 0; i < ps->n_pulse; i ++) {
    print_info( file, ps->ef[i]->W/C_cm2au );
    print_info( file, ps->ef[i]->E0 );
    print_info( file, ps->ef[i]->FWHM/C_fs2au );
    print_info( file, ps->ef[i]->Edir/M_PI );
    for (int j = 0; j < ps->n_dim; j ++)
      print_info( file, ps->ef[i]->kdir[j]/M_PI );
    fprintf( file, "\n" );
  }

  display_section( file, "help" );
  print_info( file, ps->help->w_max );

  display_section( file, "pols" );
  print_info( file, ps->pols->n_dpl );
  print_info( file, ps->pols->method_ppar );

  close_para_file( type, ps );
}

//////////////////////////// RL ////////////////////////////////
////////////////////////////////////////////////////////////
//              Specification of file format
// column no.=3: rl components in x, y and z direction
// row no.=ps->esmb->n_emsb.
////////////////////////////////////////////////////////////

void io_rl_write( parameters* ps ) {
  for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
    fprintf( ps->file->item[para_file::RL]->f[0]->fptr, "%le ", ps->pos[i_dim] );
  fprintf( ps->file->item[para_file::RL]->f[0]->fptr, "\n" );
}
void io_rl_read( parameters* ps ) {
  for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
    fscanf( ps->file->item[para_file::RL]->f[0]->fptr, "%le", &(ps->pos[i_dim]) );
}

//////////////////////////// Grid ////////////////////////////////
////////////////////////////////////////////////////////////
//              Specification of file format
// column no.=1: variable, user-defined all extra dimensions
// row no.=ps->node->n_mvar. node->n_mvar is decided by system
////////////////////////////////////////////////////////////

void io_grid_write( para_file::file_type type, double *s,
                    const char* prefix, parameters *ps )
{
  int file_idx[1] = { (int) ps->mpic->rank };
  open_para_file( type, prefix, ps, 1, NULL, file_idx, "w" );
  for (long is = 0; is < ps->node->n_mvar; is ++)
    fprintf( ps->file->item[type]->f[0]->fptr, "%le\n", s[is] );
  close_para_file( type, ps );
}

void io_grid_read( para_file::file_type type, double *s,
                   const char* prefix, parameters *ps )
{
  int file_idx[1] = { (int) ps->mpic->rank };
  open_para_file( type, prefix, ps, 1, NULL, file_idx, "r" );
  for (long is = 0; is < ps->node->n_mvar; is ++) {
    long index = ps->node->mvar_0 + is;
    fscanf( ps->file->item[type]->f[0]->fptr, "%le", &s[index] );
  }
  close_para_file( type, ps );
}

//////////////////////////// Polarization ////////////////////////////////
////////////////////////////////////////////////////////////
//              Specification of file format
// column no.=2: real and imaginary part
// row no.=(ns * nt) * n_esmb. ns includes all extra dimensions
// For the same variables, 3 files store data for 3 coordinates
////////////////////////////////////////////////////////////

void io_pol_write( para_file::file_type type, complex*** pol, parameters* ps )
{
  // here one should notice, for pullerits' method, ptot is real, one can choose
  // output only the real part to save half storage, then column no.=1
  // but when one needs post-process, one output both the real & imag parts
  int idx[2]; // { i_dpl, i_dim }
  long ns = ps->node->n_mvar;
  for (int is = 0; is < ns; is ++)
    for (int it = 0; it < ps->nt; it ++) {
      long index = is * (ps->nt) + it;
      for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
          // TODO: reverse the loop order so that we have better performance
          idx[0] = i_dpl; idx[1] = i_dim;
          FILE* fptr = get_fptr_from_idxSyn( type, idx, ps );
          fprintf( fptr, "%le %le\n",
                   real(pol[index][i_dpl][i_dim]),
                   imag(pol[index][i_dpl][i_dim]) );
          // fprintf( file[i_dim], "%le\n", real(pol[index][i_dim]) );
        }
    }
}

void io_pol_read( para_file::file_type type, complex*** pol, parameters* ps )
{
  // notice the pullerits' method one can read only a single column
  int idx[2];
  double re, im;
  long ns = ps->node->n_mvar;
  long idx0 = ps->node->mvar_0;
  if ( ps->esmb->with_old == 1 )
    idx0 = 0;
  for (int is = 0; is < ns; is ++)
    for (int it = 0; it < ps->nt; it ++) {
      // notice, idx0 is used to locate the start in global array
      long index = (idx0 + is) * (ps->nt) + it;
      for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
          // TODO: reverse the loop order so that we have better performance
          idx[0] = i_dpl; idx[1] = i_dim;
          FILE* fptr = get_fptr_from_idxSyn( type, idx, ps );
          fscanf( fptr, "%le %le", &re, &im );
          pol[index][i_dpl][i_dim] = complex( re, im );
          // seidner's method, ptot is allways real, we can save half storage
          // fscanf( file[i_dim], "%le", &re );
          // pol[index][i_dim] = complex( re, 0.0 );
        }
    }
}

// one needs two ps: one global one, one for each node.
// here, pol should be a global array, while ps is for the node
// ----------------------------------------
void io_pol_dir_write( para_file::file_type type, complex**** pol,
                       int n_dir, const char* prefix, parameters *ps )
{
  // n_dir = ps->seid->n_phase for seidner
  for (int i_dir = 0; i_dir < n_dir; i_dir ++) {
    int file_idx[2] = { i_dir, (int) ps->mpic->rank };
    open_para_file( type, prefix, ps, 2, NULL, file_idx, "w" );
    io_pol_write( type, pol[i_dir], ps );
    close_para_file( type, ps );
  }
}
void io_pol_dir_read( para_file::file_type type, complex**** pol,
                      int n_dir, const char* prefix, parameters *ps )
{
  for (int i_dir = 0; i_dir < n_dir; i_dir ++) {
    int file_idx[2] = { i_dir, (int) ps->mpic->rank };
    open_para_file( type, prefix, ps, 2, NULL, file_idx, "r" );
    io_pol_read( type, pol[i_dir], ps );
    close_para_file( type, ps );
  }
}
