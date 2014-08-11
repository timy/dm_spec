#include "mpic.h"
#include "esmb.h"
#include "mvar.h"
#include "coord.h"
#include "bath.h"
#include "repr.h"
#include "para.h"
#include "field.h"
#include "random.h"
#include "debug.h"
#include "eom.h"
#include "seidner.h"
#include "file.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>

void set_para( parameters *para, int rank=0, int size=1 )
{
    // initialization (allocation)
    para_ini( para, "cfg/parameters.cfg" );

    //#include "../cfg/parameters.h"

    // for (int ip = 0; ip < N_PULSE; ip ++) {
    //     para->ef[ip]->W = W[ip] * C_cm2au;
    //     para->ef[ip]->E0 = E0[ip];
    //     para->ef[ip]->FWHM = FWHM[ip] * C_fs2au;
    //     for (int i_dim = 0; i_dim < N_DIM; i_dim ++)
    //         para->ef[ip]->kdir[i_dim] = kdir[ip][i_dim];
    //     para->ef[ip]->Edir = Edir[ip];
    //     para->ef[ip]->tc = tc[ip];
    // }

    // set_para_coord( euler_phi, euler_theta, euler_psi, para );
    // set_para_repr( para, &para_local );
    // set_para_bath( para );
    // set_para_efield_lab( para );
    // //set_para_esmb( para );
    // //set_para_mvar( para );
    // para_mpic_set( para );
    // set_para_core( para );
    // set_para_efield_mol( para );
    // set_para_seidner( para );
    // set_para_file( para );
}
