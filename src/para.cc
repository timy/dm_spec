#include "para.h"
#include "timestep.h"
#include "esmb.h"
#include "mvar.h"
#include "mpic.h"
#include "coord.h"
#include "repr.h"
#include "bath.h"
#include "field.h"
#include "seidner.h"
#include "file.h"
#include "help.h"
#include <cstdio>
#include <cmath>
#include <cassert>
#include "debug.h"

void para_core_ini( config_t* cfg, parameters* ps );
void para_core_del( parameters* ps );

void para_init_info_write( parameters* ps );

#include "config.h"
#include <libconfig.h>

// overall
void para_ini( parameters *ps, const char* file_name, long size, long rank )
{
    // configuration file
    config_t cfg;
    config_file_ini( &cfg, file_name );

    para_core_ini( &cfg, ps );
    // sub-systems
    para_file_ini( &cfg, ps );
    para_help_ini( &cfg, ps );
    para_time_ini( &cfg, ps );
    para_esmb_ini( &cfg, ps );
    para_mvar_ini( &cfg, ps );
    para_mpic_ini( ps, size, rank );
    para_coord_ini( &cfg, ps );
    para_repr_ini( &cfg, ps );
    para_bath_ini( &cfg, ps );
    para_efield_ini( &cfg, ps );
    para_seidner_ini( &cfg, ps );


    config_file_del( &cfg );

    if (rank == 0) para_init_info_write( ps );
}
void para_del( parameters *ps )
{
    // sub-systems
    para_seidner_del( ps );
    para_efield_del( ps );
    para_bath_del( ps );
    para_repr_del( ps );
    para_coord_del( ps );
    para_mpic_del( ps );
    para_mvar_del( ps );
    para_esmb_del( ps );
    para_time_del( ps );
    para_help_del( ps );
    para_file_del( ps );
    //
    para_core_del( ps );
}

// core
void para_core_config( config_t* cfg, parameters* ps );

void para_core_ini( config_t* cfg, parameters *ps )
{
    // n_dim, n_lvl, n_dpl, n_eom
    para_core_config( cfg, ps );
    // energy, dipole, position
    ps->energy = new double [ps->n_lvl];
    ps->dipole = new double* [ps->n_dpl];
    for (int i = 0; i < ps->n_dpl; i ++)
        ps->dipole[i] = new double [ps->n_dim];
    ps->pos = new double [ps->n_dim];
}

void para_core_del( parameters *ps )
{
    delete[] ps->pos;
    for (int i = 0; i < ps->n_dpl; i ++)
        delete[] ps->dipole[i];
    delete[] ps->dipole;
    delete[] ps->energy;
}

void para_core_config( config_t* cfg, parameters* ps )
{
    config_lookup_int( cfg, "basic.n_dim", &(ps->n_dim) );
    config_lookup_int( cfg, "basic.n_lvl", &(ps->n_lvl) );
    config_lookup_int( cfg, "basic.n_dpl", &(ps->n_dpl) );
    config_lookup_int( cfg, "basic.n_eom", &(ps->n_eom) );
    assert( ps->n_dim > 0 );
    assert( ps->n_lvl > 0 );
    assert( ps->n_dpl > 0 );
    assert( ps->n_eom > 0 );
}

// output information
#define print(x) {                                              \
        fprintf( file, "%s,L%d: ", __FILE__, __LINE__ );        \
        dm_fprintf( file, #x );                                 \
        fprintf( file, " = " );                                 \
        dm_fprintf( file, x );                                  \
        fputc( '\n', file );                                    \
    }
#define display_section(x) { \
    fprintf( file, "\n%s\n", x ); }

void para_init_info_write( parameters* ps )
{
    // config.dat
    open_para_file_write( para_file::CONFIG, NULL, ps, 0, NULL );
    FILE* file = ps->file->one[para_file::CONFIG]->fptr;

    display_section( "basic" );
    print(ps->n_dim);
    print(ps->n_lvl);
    print(ps->n_dpl);
    print(ps->n_pulse);

    display_section( "time" );
    print(ps->nt);
    print(ps->t0/C_fs2au);
    print(ps->t1/C_fs2au);
    print(ps->dt/C_fs2au);
    print(ps->it0);
    print(ps->time[ps->it0]/C_fs2au);
    
    display_section( "core" );
    for (int i = 0; i < ps->n_lvl; i ++)
        print(ps->energy[i]/C_cm2au);
    for (int i = 0; i < ps->n_dpl; i ++)
        for (int j = 0; j < ps->n_dim; j ++)
            print(ps->dipole[i][j]);
    for (int i = 0; i < ps->n_dim; i ++)
        print(ps->pos[i]);
    
    display_section( "bath" );
    print(ps->bath->g);
    print(ps->bath->w_cut/C_cm2au);
    print(ps->bath->T/C_T2au);
    print(ps->bath->g12);
    print(ps->bath->g21);
    print(ps->bath->G11);
    print(ps->bath->G22);
    print(ps->bath->G12);
    print(ps->bath->G10);
    print(ps->bath->G20);
    print(ps->bath->G31);
    print(ps->bath->G32);
    print(ps->bath->G30);
    
    display_section( "repr" );
    print(ps->repr->theta);
    
    display_section( "coord" );
    print(ps->coord->phi/M_PI);
    print(ps->coord->theta/M_PI);
    print(ps->coord->psi/M_PI);
    
    display_section( "mvar" );
    print(ps->mvar->ny);
    print(ps->mvar->y0);
    print(ps->mvar->y1);
    print(ps->mvar->dy);
    print(ps->mvar->dimGrid);
    print(ps->esmb->n_esmb);

    display_section( "mpic" );
    print(ps->mpic->njob);
    print(ps->mpic->idx0);

    display_section( "efield" );
    for (int i = 0; i < ps->n_pulse; i ++) {
        print(ps->ef[i]->W/C_cm2au);
        print(ps->ef[i]->E0);
        print(ps->ef[i]->FWHM/C_fs2au);
        print(ps->ef[i]->Edir/M_PI);
        for (int j = 0; j < ps->n_dim; j ++)
            print(ps->ef[i]->kdir[j]/M_PI);
        fprintf( file, "\n" );
    }

    display_section( "help" );
    print(ps->help->w_max);

    close_para_file( para_file::CONFIG, ps );

    // time grid
    para_time_write( ps );
}
