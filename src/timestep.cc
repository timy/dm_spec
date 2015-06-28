#include "timestep.h"
#include "para.h"
#include <cstdlib>
#include <cstdio>
void para_time_config( struct config_t* cfg, struct parameters* ps );
void para_time_set( struct parameters* ps );

void para_time_ini( config_t* cfg, parameters* ps )
{
  para_time_config( cfg, ps );
  ps->time = new double [ps->nt];
  para_time_set( ps );
}

void para_time_del( parameters* ps )
{
  delete[] ps->time;
}

#include <cmath>
void para_time_set( parameters* ps )
{
  ps->dt = (ps->t1 - ps->t0) / (ps->nt - 1.0); // interval
  ps->it0 = long( (0.0 - ps->t0) / ps->dt ); // idx of t=0
  double t_it0 = ps->t0 + ps->it0 * ps->dt; // t at index it0
  double d_t0 = 0.0 - t_it0; // the shift
  ps->t0 += d_t0; // right shift overall grid
  ps->t1 += d_t0;
  for (long it = 0; it < ps->nt; it ++)
    ps->time[it] = ps->t0 + it * ps->dt;
  // estimate the max freq from time interval
  ps->help->w_max = M_PI / ps->dt / C_cm2au;
}

#include "file.h"
void para_time_write( parameters* ps )
{
  open_para_file( para_file::TIME, NULL, ps, 0, NULL, NULL, "w" );
  for (long it = 0; it < ps->nt; it ++)
    fprintf( ps->file->item[para_file::TIME]->f[0]->fptr,
             "%le\n", ps->time[it] / C_fs2au );
  close_para_file( para_file::TIME, ps );
}

#include <libconfig.h>
void para_time_config( config_t* cfg, parameters* ps )
{
  int nt;
  config_lookup_int( cfg, "time.nt", &nt );
  config_lookup_float( cfg, "time.t0", &(ps->t0) );
  config_lookup_float( cfg, "time.t1", &(ps->t1) );
  ps->nt = nt;
  ps->t0 *= C_fs2au;
  ps->t1 *= C_fs2au;
}
