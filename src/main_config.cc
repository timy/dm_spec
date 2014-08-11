#include "para.h"
#include "debug.h"
#include "config.h"
#include "libconfig.h"
#include <cmath>
#include <cassert>



void config_euler( config_t *cfg )
{
    para_coord *coord = new para_coord;

    config_lookup_float( cfg, "euler.phi", &(coord->phi) );
    config_lookup_float( cfg, "euler.theta", &(coord->theta) );
    config_lookup_float( cfg, "euler.psi", &(coord->psi) );

    coord->phi *= M_PI;
    coord->theta *= M_PI;
    coord->psi *= M_PI;
    // print(coord->phi);
    // print(coord->theta);
    // print(coord->psi);
    delete coord;
}

void config_dimer( config_t *cfg )
{
    double J;
    config_setting_t *setting_1, *setting_2;
    setting_1 = config_lookup( cfg, "dimer.E" );
    int n_E = config_setting_length( setting_1 );
    assert( n_E == 2 );
    double *E_local = new double [n_E];
    for (int i_E = 0; i_E < n_E; i_E ++) {
        E_local[i_E] = config_setting_get_float_elem( setting_1, i_E );
        // print(E_local[i_E]);
    }

    config_lookup_float( cfg, "dimer.J", &J );

    setting_1 = config_lookup( cfg, "dimer.mu" );
    int n_mu = config_setting_length( setting_1 );

    double **mu = new double* [n_mu];
    for (int i_mu = 0; i_mu < n_mu; i_mu ++) {
        setting_2 = config_setting_get_elem( setting_1, i_mu );
        int n_dim = config_setting_length( setting_2 );
        assert( n_dim == N_DIM );
        mu[i_mu] = new double[n_dim];
        for (int i_dim = 0; i_dim < n_dim; i_dim ++) {
            mu[i_mu][i_dim] =
                config_setting_get_float_elem( setting_2, i_dim );
            // print(mu[i_mu][i_dim]);
        }
    }
    
    for (int i_mu = 0; i_mu < n_mu; i_mu ++)
        delete mu[i_mu];
    delete[] mu;

    delete[] E_local;
}

void config_position( config_t *cfg )
{
    config_setting_t *setting;
    setting = config_lookup( cfg, "position" );
    int n_dim = config_setting_length( setting );
    assert( n_dim == N_DIM );
    double *pos = new double [n_dim];
    for (int i_dim = 0; i_dim < n_dim; i_dim ++) {
        pos[i_dim] = config_setting_get_float_elem( setting, i_dim );
        // print( pos[i_dim] );
    }
    delete[] pos;
}

void config_field( config_t *cfg )
{
    config_setting_t *setting;
    setting = config_lookup( cfg, "field" );
    int n_pulse = config_setting_length( setting );
    para_ef **ef = new para_ef* [n_pulse];

    for (int i_pulse = 0; i_pulse < n_pulse; i_pulse ++) {
        ef[i_pulse] = new para_ef;
        config_setting_t *pulse =
            config_setting_get_elem( setting, i_pulse );
        config_setting_lookup_float( pulse, "W", &(ef[i_pulse]->W) );
        config_setting_lookup_float( pulse, "E0", &(ef[i_pulse]->E0) );
        config_setting_lookup_float( pulse, "FWHM", &(ef[i_pulse]->FWHM) );
        config_setting_t *kdir = config_setting_get_member( pulse, "kdir" );
        int n_dim = config_setting_length( kdir );
        assert( n_dim == N_DIM );
        ef[i_pulse]->kdir = new double [n_dim];
        for (int i_dim = 0; i_dim < n_dim; i_dim ++) {
            ef[i_pulse]->kdir[i_dim] =
                config_setting_get_float_elem( kdir, i_dim );
            // print( ef[i_pulse]->kdir[i_dim] );
        }
        // NOTE: Edir has not yet been read!!! 
        config_setting_lookup_float( pulse, "tc", &(ef[i_pulse]->tc) );
        // print( ef[i_pulse]->W );
        // print( ef[i_pulse]->E0 );
        // print( ef[i_pulse]->FWHM );
        // print( ef[i_pulse]->tc );
     }


    for (int i_pulse = 0; i_pulse < n_pulse; i_pulse ++) {
        delete[] ef[i_pulse]->kdir;
        delete ef[i_pulse];
    }
    delete[] ef;
 }

void config_bath( config_t *cfg )
{
    para_bath *bath = new para_bath;

    config_lookup_float( cfg, "bath.g", &(bath->g) );
    config_lookup_float( cfg, "bath.w_cut", &(bath->w_cut) );
    config_lookup_float( cfg, "bath.T", &(bath->T) );
    // print( bath->g );
    // print( bath->w_cut );
    // print( bath->T );

    delete bath;
}

void config_time( config_t *cfg )
{
    int nt;
    double t0, t1;

    config_lookup_int( cfg, "time.nt", &nt );
    config_lookup_float( cfg, "time.t0", &t0 );
    config_lookup_float( cfg, "time.t1", &t1 );

    // print(nt);
    // print(t0);
    // print(t1);
}

void config_file( config_t *cfg )
{
    const char *name = NULL;
    char dm[256];
    
    config_lookup_string( cfg, "file.dm", &name );
    strcpy( dm, name );
    print( dm );
}


void config_seidner( config_t *cfg )
{    
    config_setting_t *setting;
    setting = config_lookup( cfg, "seidner.index" );
    int n_phase = config_setting_length( setting );
    
    int** index = new int* [n_phase];
    for (int i_phase = 0; i_phase < n_phase; i_phase ++) {
        config_setting_t *elem =
            config_setting_get_elem( setting, i_phase );
        int n_dim = config_setting_length( elem );
        assert( n_dim == 3 );
        index[i_phase] = new int [n_dim];
        for (int i_dim = 0; i_dim < n_dim; i_dim ++) {
            index[i_phase][i_dim] = config_setting_get_int_elem( elem, i_dim );
            // fprintf( stdout, "%d\t", index[i_phase][i_dim] );
        }
        //fprintf( stdout, "\n" );
    }
    
    for (int i_phase = 0; i_phase < n_phase; i_phase ++)
        delete[] index[i_phase];
    delete[] index;

}

void config_esmb( config_t* cfg )
{
    long n_esmb;
    config_lookup_int( cfg, "esmb.n_esmb", (int*)&n_esmb );
    print(n_esmb);
}

 // void set_para( parameters *para, int rank=0, int size=1 );

 int main( int argc, char* argv[] )
 {
     config_t cfg;
     // parameters ps;
     // set_para( &ps );

     config_file_ini( &cfg, "cfg/parameters.cfg" );

     config_euler( &cfg );
     config_dimer( &cfg );
     config_position( &cfg );
     
     config_field( &cfg );
     config_bath( &cfg );
     config_time( &cfg );
     config_file( &cfg );
     config_seidner( &cfg );
     config_esmb( &cfg );
     config_file_del( &cfg );

     // del_para( &ps );

    return 0;
}
