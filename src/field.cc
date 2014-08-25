#include "rotation.h"
#include "coord.h"
#include "para.h"
#include "field.h"
#include <complex>
#include <cmath>
#include <cstdio>

#define complex std::complex<double>
#define EYE complex(0.0, 1.0)


void para_efield_config( struct config_setting_t* setting, struct parameters* ps );

#include <cassert>
#include <libconfig.h>
void para_efield_ini( config_t* cfg, parameters* ps )
{
    config_setting_t *setting;
    setting = config_lookup( cfg, "field" );
    ps->n_pulse = config_setting_length( setting );
    assert( ps->n_pulse > 0 );
    ps->ef = new para_ef*[ps->n_pulse];
    for (int ip = 0; ip < ps->n_pulse; ip ++) {
        ps->ef[ip] = new para_ef;
        ps->ef[ip]->kdir = new double[ps->n_dim];
        ps->ef[ip]->kuvL = new double[ps->n_dim];
        ps->ef[ip]->EuvL = new double[ps->n_dim];
        ps->ef[ip]->kuvM = new double[ps->n_dim];
        ps->ef[ip]->EuvM = new double[ps->n_dim];
    }

    para_efield_config( setting, ps );
    set_para_efield_lab( ps );
    set_para_efield_mol( ps );
}

void para_efield_del( parameters *ps )
{
    for (int ip = 0; ip < ps->n_pulse; ip ++) {
        delete[] ps->ef[ip]->EuvM;
        delete[] ps->ef[ip]->kuvM;
        delete[] ps->ef[ip]->EuvL;
        delete[] ps->ef[ip]->kuvL;
        delete[] ps->ef[ip]->kdir;
        delete ps->ef[ip];
    }
    delete[] ps->ef;
}

#include "debug.h"
void para_efield_config( config_setting_t* setting, parameters* ps )
{
    for (int ip = 0; ip < ps->n_pulse; ip ++) {
        config_setting_t *pulse =
            config_setting_get_elem( setting, ip );
        config_setting_lookup_float( pulse, "W", &(ps->ef[ip]->W) );
        config_setting_lookup_float( pulse, "E0", &(ps->ef[ip]->E0) );
        config_setting_lookup_float( pulse, "FWHM", &(ps->ef[ip]->FWHM) );
        config_setting_t *kdir = config_setting_get_member( pulse, "kdir" );
        assert( config_setting_length( kdir ) == ps->n_dim );
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            ps->ef[ip]->kdir[i_dim] =
                config_setting_get_float_elem( kdir, i_dim );
        config_setting_lookup_float( pulse, "Edir", &(ps->ef[ip]->Edir) );
        config_setting_lookup_float( pulse, "tc", &(ps->ef[ip]->tc) );

        ps->ef[ip]->W *= C_cm2au;
        ps->ef[ip]->FWHM *= C_fs2au;
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            ps->ef[ip]->kdir[i_dim] *= M_PI;
        ps->ef[ip]->Edir *= M_PI;
        ps->ef[ip]->tc *= C_fs2au;
     }
}

#include <cstdlib>
void set_para_efield_lab( parameters *ps )
{
    // the following parameters are set: k0, kuvL, EuvL
    // amplitude of wave vectors
    for (int ip = 0; ip < ps->n_pulse; ip ++)
        ps->ef[ip]->k0 = C_om2wv * (ps->ef[ip]->W);

    double **mxRotB_k = new double*[ps->n_dim];
    double **mxRotF_k = new double*[ps->n_dim];
    double **mxRotB_E = new double*[ps->n_dim];
    double **mxRotF_E = new double*[ps->n_dim];
    for (int i = 0; i < ps->n_dim; i ++) {
        mxRotB_k[i] = new double[ps->n_dim];
        mxRotF_k[i] = new double[ps->n_dim];
        mxRotB_E[i] = new double[ps->n_dim];
        mxRotF_E[i] = new double[ps->n_dim];
    }
    double *ref_E = new double[ps->n_dim];

    double const ini_vec[3] = { 0.0, 0.0, 1.0 };
    for (int ip = 0; ip < ps->n_pulse; ip ++) {
        // unit vector of wave vectors
        rotation_matrix( ps->ef[ip]->kdir[0],
                         ps->ef[ip]->kdir[1],
                         ps->ef[ip]->kdir[2],
                         mxRotB_k, mxRotF_k );
        // rotate (0,0,1) by defined Euler angle
        rotate_transform( ini_vec, ps->ef[ip]->kuvL, mxRotF_k );

        // unit vector of electric field based on the wave vector
        // electric field should be perpendicular to wave vector
        // 1st, we are in k ref coord where k points to (0,0,1)
        // 2nd, rotate (0, 0, 1) by phi = Edir, theta = PI/2 (perp)
        rotation_matrix( ps->ef[ip]->Edir, 0.5*M_PI, 0.0, mxRotB_E, mxRotF_E );
        rotate_transform( ini_vec, ref_E, mxRotF_E );
        // 3rd, rotate from k ref coord back to lab coord
        // ref_E in ref coord is known, we want coord in lab,
        // forward transformation is the option
        rotate_transform( ref_E, ps->ef[ip]->EuvL, mxRotF_k );
    }
    for (int i = 0; i < ps->n_dim; i ++) {
        delete[] mxRotB_k[i]; delete[] mxRotF_k[i];
        delete[] mxRotB_E[i]; delete[] mxRotF_E[i];
    }
    delete[] mxRotB_k; delete[] mxRotF_k;
    delete[] mxRotB_E; delete[] mxRotF_E;
    delete[] ref_E;
}

void set_para_efield_mol( parameters *ps )
{
    // the following parameters are set: kuvM, EuvM
    // unit vectors: transform from lab frame to mol frame
    for (int ip = 0; ip < ps->n_pulse; ip ++) {
        coord_from_lab_to_mol( ps->ef[ip]->kuvL, ps->ef[ip]->kuvM, ps );
        coord_from_lab_to_mol( ps->ef[ip]->EuvL, ps->ef[ip]->EuvM, ps );
    }
}

// ----------------------------------------

double field_real( double t, para_ef *f )
{   // units in a.u.; 4*log(2) = 2.77259
    double dt = t - (f->tc);
    double Ef = (f->E0)
        * exp( - 2.772588722 * dt * dt / ( (f->FWHM) * (f->FWHM) ) )
        * cos( (f->W) * dt - (f->ph) );
    return Ef;
}

void field_real_components( double *ef, double t, parameters *ps )
{
    double amp[3];
    // time-dependent amplitude of each pulse
    for (int ip = 0; ip < ps->n_pulse; ip ++) {
        // ps->ef[ip]->ph = ps->phiM[iphi][ip];
        amp[ip] = field_real( t, ps->ef[ip] );
    }
    // components of each dimension in mol coordinates
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        ef[i_dim] = 0.0;
        for (int ip = 0; ip < ps->n_pulse; ip ++)
            ef[i_dim] += amp[ip] * ps->ef[ip]->EuvM[i_dim];
    }
}

// ------------------------------------------

complex field_cmplx( double t, para_ef *f )
{   // units in a.u.; 4*log(2) = 2.77259
    double dt = t - (f->tc);
    complex Ef = (f->E0)
        * exp( - 2.77259 * dt * dt / ( (f->FWHM) * (f->FWHM) ) )
        * 0.5 * exp( - EYE * ( (f->W) * dt - (f->ph) ) );
    return Ef;
}

complex field_cmplx_RWA( double t, para_ef *f )
{   // units in a.u.; 4*log(2) = 2.77259
    double dt = t - (f->tc);
    complex Ef = (f->E0)
        * exp( - 2.77259 * dt * dt / ( (f->FWHM) * (f->FWHM) ) )
        * 0.5 * exp( EYE * (f->ph) );
    return Ef;
}
