#include "node.h"
#include "para.h"
#include "debug.h"
#include <libconfig.h>

// this module must be after mpic and mvar
void para_node_ini( config_t* cfg, parameters* ps )
{
  ps->node = new para_node;
  para_node_set( ps );
}

void para_node_del( parameters* ps )
{
  delete ps->node;
}

void para_node_set( parameters* ps )
{
  // init values, may be overwritten
  ps->node->n_esmb = ps->esmb->n_esmb;
  ps->node->esmb_0 = 0;

  ps->node->n_mvar = ps->mvar->ny;
  ps->node->mvar_0 = 0;

  if (ps->mpic->partition == para_mpic::GRID) {
    ps->node->n_mvar = ps->mpic->njob;
    ps->node->mvar_0 = ps->mpic->idx0;
  } else if (ps->mpic->partition == para_mpic::ESMB) {
    ps->node->n_esmb = ps->mpic->njob;
    ps->node->esmb_0 = ps->mpic->idx0;
  }
}
