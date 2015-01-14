#include "para.h"
#include <cmath>

double re_FT_CF( double w, parameters *ps );
double bath_gamma( double w, parameters *ps );

void bath_set_const_dimer( parameters *ps )
{
    double c = cos( 2.0 * (ps->repr->theta) );
    double s = sin( 2.0 * (ps->repr->theta) );
    double re_ftcf_0 = re_FT_CF( 0.0, ps );
    double GmRate11 = 2.0*c*c * re_ftcf_0;
    double GmRate10 = ( 1.0 - 0.5*s*s ) * re_ftcf_0;
    double GmRate21 = GmRate10;
    double GmRate20 = 2.0 * re_ftcf_0;

    double w12 = (ps->energy->energy[1]) - (ps->energy->energy[2]);

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

double spectral_density_J( double w, parameters *ps )
{
    double g2 = (ps->bath->g) * (ps->bath->g);
    return g2 * w / (ps->bath->w_cut) * exp( -w / (ps->bath->w_cut) );
}

double distribution_boson_n( double w, parameters *ps )
{
    return 1.0 / ( exp( w / ( (ps->bath->T) ) ) - 1.0 );
}

// real part of Fourier transform of the correlation function
double re_FT_CF( double w, parameters *ps )
{
    double re_FTCF;
    if ( fabs(w) < 1e-6 )
        re_FTCF = (ps->bath->g) * (ps->bath->g) * (ps->bath->T) / (ps->bath->w_cut);
    else if ( w > 0.0 )
        re_FTCF = distribution_boson_n( w, ps ) * spectral_density_J( w, ps );
    else // w < 0.0
        re_FTCF = (1.0 + distribution_boson_n( -w, ps )) * spectral_density_J( -w, ps );
    return re_FTCF;
}

double bath_gamma( double w, parameters *ps )
{
    double s = sin( 2.0 * (ps->repr->theta) );
    double gm = s * s * re_FT_CF( w, ps );
    return gm;
}

#include <libconfig.h>
void para_bath_const_dimer_config( struct config_t* cfg, struct parameters *ps )
{
    config_lookup_float( cfg, "bath.const_dimer.g", &(ps->bath->g) );
    config_lookup_float( cfg, "bath.const_dimer.w_cut", &(ps->bath->w_cut) );
    config_lookup_float( cfg, "bath.const_dimer.T", &(ps->bath->T) );
    ps->bath->w_cut *= C_cm2au;
    ps->bath->T *= C_T2au;
}
