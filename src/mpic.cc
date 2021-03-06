#include "mpic.h"
#include "para.h"
#include "debug.h"
#include <libconfig.h>

void para_mpic_config( config_t* cfg, parameters* ps );
long assign_jobs( long rank, long size, long total, long *start );

#include "file.h"
void para_mpic_ini( config_t* cfg, parameters *ps, long size, long rank )
{
  ps->mpic = new para_mpic;
  para_mpic_config( cfg, ps );
  para_mpic_set( ps, size, rank );

  int file_idx[1] = { (int) ps->mpic->rank };
  open_para_file( para_file::LOG, NULL, ps, 1, NULL, file_idx, "w" );
}

void para_mpic_del( parameters *ps )
{
  close_para_file( para_file::LOG, ps );
  delete ps->mpic;
}

void para_mpic_set( parameters *ps, long size, long rank )
{
  ps->mpic->size = size;
  ps->mpic->rank = rank;

  long n_total;
  if (ps->mpic->partition == para_mpic::GRID) {
    n_total = ps->mvar->ny;
  } else if (ps->mpic->partition == para_mpic::ESMB) {
    n_total = ps->esmb->n_esmb;
  }

  ps->mpic->njob = assign_jobs( ps->mpic->rank, ps->mpic->size,
                                n_total, &(ps->mpic->idx0) );
}

void para_mpic_config( config_t* cfg, parameters* ps )
{
  const char* name = NULL;
  config_lookup_string( cfg, "mpic.partition", &name );
  if (strcmp(name, "grid") == 0) {
    ps->mpic->partition = para_mpic::GRID;
  } else if (strcmp(name, "esmb") == 0) {
    ps->mpic->partition = para_mpic::ESMB;
  } else {
    fprintf( stderr, "mpic.partition '%s' unknown. Check "
             "section 'mpic' in your configure file\n", name );
  }
}

long assign_jobs( long rank, long size, long total, long *start )
{   // calculation for load balance
  long n_task = total / size;
  if ( total % size == 0 )
    *start = n_task * rank;
  else {
    n_task ++;
    long n_less = n_task * size - total;
    long n_more = size - n_less;
    if (rank < n_more)
      *start = n_task * rank;
    else {
      *start = n_task * n_more + (n_task - 1) * (rank - n_more);
      n_task --;
    }
  }
  return n_task;
}
