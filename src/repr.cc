#include "para.h"
#include "repr.h"
#include <cmath>
#include <cstdio>
#include <cassert>

void para_repr_config( config_t* cfg, parameters* ps );
void para_repr_dimer_config( config_t *cfg, parameters* ps );
void para_repr_generic_config( config_t *cfg, parameters* ps );

#include <cstring>
#include <libconfig.h>
#include "debug.h"

void para_repr_ini( config_t* cfg, parameters* ps )
{
  ps->repr = new para_repr;
  para_repr_config( cfg, ps );

  if (ps->repr->type == para_repr::GENERIC) {
    para_repr_generic* ptr = new para_repr_generic;
    ptr->E = new double [ps->n_lvl];
    ptr->mu = new double* [ps->n_dpl];
    for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++) {
      ptr->mu[i_dpl] = new double [ps->n_dim];
    }
    ps->repr->ptrSt = ptr;
    para_repr_generic_config( cfg, ps );
  } else if (ps->repr->type == para_repr::DIMER) {
    ps->repr->ptrSt = new para_repr_dimer_local;
    para_repr_dimer_config( cfg, ps );
  }

  set_para_repr( ps );
}

void para_repr_del( parameters *ps )
{
  if (ps->repr->type == para_repr::GENERIC) {
    para_repr_generic* ptr = (para_repr_generic*) ps->repr->ptrSt;
    for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++)
      delete[] ptr->mu[i_dpl];
    delete[] ptr->mu;
    delete[] ptr->E;
    delete ptr;
  } else if (ps->repr->type == para_repr::DIMER) {
    delete ps->repr->ptrSt;
  }
  delete ps->repr;
}

#include <cstring>
void para_repr_config( struct config_t* cfg, struct parameters* ps )
{
  const char* name = NULL;
  config_lookup_string( cfg, "repr.type", &name );
  if (strcmp(name, "generic") == 0) {
    ps->repr->type = para_repr::GENERIC;
  } else if (strcmp(name, "dimer") == 0) {
    ps->repr->type = para_repr::DIMER;
  } else {
    fprintf( stderr, "repr.type '%s' unknown. Check "
             "section 'repr' in your configure file\n", name );
  }
}

void repr_set_exciton_dimer( parameters *ps );
void repr_set_generic( parameters *ps );

void set_para_repr( parameters *ps )
{
  if (ps->repr->type == para_repr::GENERIC) {
    repr_set_generic( ps );
  } else if (ps->repr->type == para_repr::DIMER) {
    repr_set_exciton_dimer( ps );
  }
}
