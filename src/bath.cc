#include "para.h"
#include "bath.h"
#include <cmath>

void prepare_bath_para( parameters *ps );

void para_bath_config( struct config_t* cfg, struct parameters *ps );
#include <libconfig.h>
void para_bath_ini( config_t* cfg, parameters *ps )
{
    ps->bath = new para_bath;
    para_bath_config( cfg, ps );
    set_para_bath( ps );
}

void para_bath_del( parameters *ps )
{
    delete ps->bath;
}

#include "debug.h"
void para_bath_config( struct config_t* cfg, struct parameters *ps )
{
    config_lookup_float( cfg, "bath.g", &(ps->bath->g) );
    config_lookup_float( cfg, "bath.w_cut", &(ps->bath->w_cut) );
    config_lookup_float( cfg, "bath.T", &(ps->bath->T) );
    ps->bath->w_cut *= C_cm2au;
    ps->bath->T *= C_T2au;
}

void set_para_bath( parameters *ps )
{
    prepare_bath_para( ps );
}

double spectral_density_J( double w, parameters *ps )
{
    double g2 = (ps->bath->g) * (ps->bath->g);
    double J = g2 * w / (ps->bath->w_cut) * exp( -w / (ps->bath->w_cut) );
    return J;
}

double boson_n( double w, parameters *ps )
{
    double bsn = 1.0 / ( exp( w / ( (ps->bath->T) ) ) - 1.0 );
    return bsn;
}

// real part of Fourier transform of the correlation function
double re_FT_CF( double w, parameters *ps )
{
    double re_FTCF;
    if ( fabs(w) < 1e-6 )
        re_FTCF = (ps->bath->g) * (ps->bath->g) * (ps->bath->T) / (ps->bath->w_cut);
    else if ( w > 0.0 )
        re_FTCF = boson_n( w, ps ) * spectral_density_J( w, ps );
    else // w < 0.0
        re_FTCF = (1.0 + boson_n( -w, ps )) * spectral_density_J( -w, ps );
    return re_FTCF;
}

double bath_gamma( double w, parameters *ps )
{
    double s = sin( 2.0 * (ps->repr->theta) );
    double gm = s * s * re_FT_CF( w, ps );
    return gm;
}

void prepare_bath_para( parameters *ps )
{
    double c = cos( 2.0 * (ps->repr->theta) );
    double s = sin( 2.0 * (ps->repr->theta) );
    double re_ftcf_0 = re_FT_CF( 0.0, ps );
    double GmRate11 = 2.0*c*c * re_ftcf_0;
    double GmRate10 = ( 1.0 - 0.5*s*s ) * re_ftcf_0;
    double GmRate21 = GmRate10;
    double GmRate20 = 2.0 * re_ftcf_0;

    double w12 = (ps->energy[1]) - (ps->energy[2]);

    ps->bath->g12 = bath_gamma(  w12, ps );
    ps->bath->g21 = bath_gamma( -w12, ps );
    ps->bath->G11 = ps->bath->g21;
    ps->bath->G22 = ps->bath->g12;
    ps->bath->G12 = GmRate11 + 0.5 * ( ps->bath->g12 + ps->bath->g21 );
    ps->bath->G10 = GmRate10 + 0.5 * ps->bath->g21;
    ps->bath->G20 = GmRate10 + 0.5 * ps->bath->g12;
    ps->bath->G31 = GmRate21 + 0.5 * ps->bath->g21;
    ps->bath->G32 = GmRate21 + 0.5 * ps->bath->g12;
    ps->bath->G30 = GmRate20;
}
