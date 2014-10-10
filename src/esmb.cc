#include "esmb.h"
#include "para.h"
#include "seidner.h"
#include <gsl/gsl_rng.h>

void para_esmb_config( config_t* cfg, parameters* ps );
void para_esmb_ini( config_t* cfg, parameters* ps )
{
    ps->esmb = new para_esmb;
    para_esmb_config( cfg, ps );
    ps->esmb->rng = (void*) gsl_rng_alloc( gsl_rng_taus2 );
}

void para_esmb_del( parameters *ps )
{
    gsl_rng_free( (gsl_rng*) ps->esmb->rng );
    delete ps->esmb;
}

#include <libconfig.h>
void para_esmb_config( config_t* cfg, parameters* ps )
{
    int n_esmb, with_old;
    config_lookup_int( cfg, "esmb.n_esmb", &n_esmb );
    config_lookup_int( cfg, "esmb.with_old", &with_old );
    ps->esmb->n_esmb = n_esmb;
    ps->esmb->with_old = with_old;
}

#include "bath.h"
#include "repr.h"
#include "util.h"
#include "coord.h"
#include "random.h"
#include "output.h"
#include "field.h"
#include <cmath>
#include <cstdlib>

void para_esmb_update( long i_esmb, parameters *ps )
{
    // Orientation
    ps->coord->phi   = 2.0 * M_PI * // [0, 2 pi]
        gsl_rng_uniform( (gsl_rng*) ps->esmb->rng );
    ps->coord->theta = 2.0 * // [0, pi)
        asin( sqrt( gsl_rng_uniform( (gsl_rng*) ps->esmb->rng ) ) );
    ps->coord->psi   = 2.0 * M_PI *
        gsl_rng_uniform( (gsl_rng*) ps->esmb->rng );
    set_para_coord( ps );

    // try to output the orientation, let's see results
    // output_mol_orient( ps->file->one[para_file::ORIENT]->fptr, ps );

    if (ps->repr->type == para_repr::GENERIC) {
    } else if (ps->repr->type == para_repr::DIMER) {
        repr_set_exciton_dimer( ps, (para_repr_dimer_local*) ps->repr->ptrSt );
    }

    set_para_bath( ps );
    set_para_efield_lab( ps );
    set_para_efield_mol( ps );

    // positions
    switch (ps->pols->method_ppar) {
    case para_pols::PULLERITS:
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
            double r = gsl_rng_uniform( (gsl_rng*) ps->esmb->rng );
            ps->pos[i_dim] = (r - 0.5) * 2e-4;
        }
        break;
    case para_pols::SEIDNER:
        set_para_seidner( ps );
        break;
    }

    // set_para_mvar( ps );
    // para_mpic_set( ps, size, rank );
    // para_time_set( ps );
    // set_para_file( ps );

    // output information after sample update
    if (ps->mpic->rank == 0 && i_esmb == 0)
        output_info( para_file::INFO, ps );
}
