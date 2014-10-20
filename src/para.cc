#include "energy.h"
#include "dipole.h"
#include "para.h"
#include "position.h"
#include "timestep.h"
#include "esmb.h"
#include "mvar.h"
#include "mpic.h"
#include "node.h"
#include "coord.h"
#include "repr.h"
#include "bath.h"
#include "field.h"
#include "seidner.h"
#include "pols.h"
#include "file.h"
#include "help.h"
#include "output.h"
#include <cstdio>
#include <cmath>
#include <cassert>
#include "debug.h"

void para_core_ini( config_t* cfg, parameters* ps );
void para_core_del( parameters* ps );

void para_init_config_write( parameters* ps );

#include "config.h"
#include <libconfig.h>

// overall
void para_ini( parameters *ps, const char* dirBase, long size, long rank )
{
    char file_name[1024];
    strcpy( ps->dirBase, dirBase );
    strcpy( file_name, ps->dirBase );
    strcat( file_name, "cfg/parameters.cfg" );

    // configuration file
    config_t cfg;
    config_file_ini( &cfg, file_name );

    para_core_ini( &cfg, ps );
    para_pos_ini( &cfg, ps );
    // sub-systems
    para_energy_ini( &cfg, ps );
    para_dipole_ini( &cfg, ps );
    para_pols_ini( &cfg, ps );
    para_file_ini( &cfg, ps );
    para_help_ini( &cfg, ps );
    para_time_ini( &cfg, ps );
    para_esmb_ini( &cfg, ps );
    para_mvar_ini( &cfg, ps );
    para_mpic_ini( &cfg, ps, size, rank );
    para_node_ini( &cfg, ps );
    para_coord_ini( &cfg, ps );
    para_repr_ini( &cfg, ps );
    para_bath_ini( &cfg, ps );
    para_efield_ini( &cfg, ps );
    para_seidner_ini( &cfg, ps );

    config_file_del( &cfg );

    // output contents read from config file: 'config.dat'
    // may further modified, should compare with 'info.dat'
    if (rank == 0) {
        output_info( para_file::CONFIG, ps );
        para_time_write( ps );
    }
}
void para_del( parameters *ps )
{
    // sub-systems
    para_seidner_del( ps );
    para_efield_del( ps );
    para_bath_del( ps );
    para_repr_del( ps );
    para_coord_del( ps );
    para_node_del( ps );
    para_mpic_del( ps );
    para_mvar_del( ps );
    para_esmb_del( ps );
    para_time_del( ps );
    para_help_del( ps );
    para_file_del( ps );
    para_pols_del( ps );
    para_dipole_del( ps );
    para_energy_del( ps );
    //
    para_pos_del( ps );
    para_core_del( ps );
}

// core
void para_core_config( config_t* cfg, parameters* ps );

void para_core_ini( config_t* cfg, parameters *ps )
{
    // n_dim, n_lvl, n_dpl, n_eom
    para_core_config( cfg, ps );
}

void para_core_del( parameters *ps )
{
}

void para_core_config( config_t* cfg, parameters* ps )
{
    config_lookup_int( cfg, "basic.n_dim", &(ps->n_dim) );
    config_lookup_int( cfg, "basic.n_lvl", &(ps->n_lvl) );
    config_lookup_int( cfg, "basic.n_dpl", &(ps->n_dpl) );
    ps->n_eom = ps->n_lvl * ps->n_lvl;
    assert( ps->n_dim > 0 );
    assert( ps->n_lvl > 0 );
    assert( ps->n_dpl > 0 );
}
