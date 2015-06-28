#include "para.h"
#include "bath.h"
#include "complex.h"
#define complex std::complex<double>

complex rf_gamma_plus( int l, int n, int m, int k, parameters* ps );
complex rf_gamma_minus( int l, int n, int m, int k, parameters* ps );
complex ft_correlation( int r, int s, double w, parameters* ps );
bool secular_condition( int m, int n, int k, int l, parameters* ps );

void bath_set_const_redfield_calc( parameters* ps )
{
  for (int m = 0; m < ps->n_lvl; m ++)
    for (int n = 0; n < ps->n_lvl; n ++)
      for (int k = 0; k < ps->n_lvl; k ++)
        for (int l = 0; l < ps->n_lvl; l ++) {
          complex R = 0.0;
          if (secular_condition( m, n, k, l, ps ) == true) {
            R = rf_gamma_plus(l, n, m, k, ps) + rf_gamma_minus(l, n, m, k, ps);
            if (n == l)
              for (int a = 0; a < ps->n_lvl; a ++)
                R -= rf_gamma_plus(m, a, a, k, ps);
            if (k == m)
              for (int a = 0; a < ps->n_lvl; a ++)
                R -= rf_gamma_minus(l, a, a, n, ps);
          }
          ps->bath->redfield_tensor[m][n][k][l] = R;
        }
}

bool secular_condition( int m, int n, int k, int l, parameters* ps )
{
  double w_mn = ps->energy->energy[m] - ps->energy->energy[n];
  double w_kl = ps->energy->energy[k] - ps->energy->energy[l];
  bool result = false;
  if ( fabs(w_mn - w_kl) < 1e-6 )
    result = true;
  return result;
}

complex rf_gamma_plus( int l, int n, int m, int k, parameters* ps )
{
  complex gamma = 0.0;
  int n_site = ps->n_lvl;
  for (int r = 0; r < n_site; r ++)
    for (int s = 0; s < n_site; s++) {
      // TODO: here the system operator matrix should be multplied
      // complex s_ln = repr_transform_operator( l, n, op_S, c, ps );
      // complex s_mk = repr_transform_operator( m, k, op_S, c, ps );

      if (r==l && r==n && s==m && s==k) { // currently use a delta function
        double w_mk = ps->energy->energy[m] - ps->energy->energy[k];
        gamma += ft_correlation( r, s, w_mk, ps );
      }
    }
  return gamma;
}

complex rf_gamma_minus( int l, int n, int m, int k, parameters* ps )
{
  complex gamma = 0.0;
  int n_site = ps->n_lvl;
  for (int r = 0; r < n_site; r ++)
    for (int s = 0; s < n_site; s++) {
      // TODO: here the system operator matrix should be multplied
      if (r==l && r==n && s==m && s==k) { // currently use a delta function
        double w_ln = ps->energy->energy[l] - ps->energy->energy[n];
        gamma += ft_correlation( s, r, w_ln, ps );
      }
    }
  return gamma;
}

complex ft_correlation( int r, int s, double w, parameters* ps )
{
  // TODO: here only give the real part, imaginary needs principle part
  // TODO: correlation from different sites may also be interesting
  complex ftCf;
  if (r == s) { // correlation from the same site
    if (fabs(w) < 1e-6) {
      ftCf = ps->bath->g * ps->bath->g * ps->bath->T / ps->bath->w_cut;
    } else if (w > 0) {
      ftCf = spectral_density_J( w, ps ) * distribution_boson_n( w, ps );
    } else if (w < 0) {
      ftCf = spectral_density_J( -w, ps ) * ( 1.0 + distribution_boson_n( -w, ps ) );
    }
  } else { // correlation from differents sites is neglected
    ftCf = 0.0;
  }
  return ftCf;
}

#include <libconfig.h>
void para_bath_const_redfield_calc_config( struct config_t* cfg, struct parameters* ps )
{
  config_lookup_float( cfg, "bath.const_redfield_calc.g", &(ps->bath->g) );
  config_lookup_float( cfg, "bath.const_redfield_calc.w_cut", &(ps->bath->w_cut) );
  config_lookup_float( cfg, "bath.const_redfield_calc.T", &(ps->bath->T) );
  ps->bath->w_cut *= C_cm2au;
  ps->bath->T *= C_T2au;
}


// complex repr_transform_operator( int l, int n, complex** op_S, complex** c, parameters* ps )
// {
//     // c: transformation matrix between basis
//     complex s_ln = 0.0;
//     for (int ll = 0; ll < ps->n_lvl; ll ++) {
//         for (int nn = 0; nn < ps->n_lvl; nn ++) {
//             s_ln += conj( c[ll][l] ) * c[nn][n] * op_S[ll][nn];
//         }
//     }
//     return s_ln;
// }

// void repr_transform_matrix( complex ** c1, complex** c2, parameters* ps )
// {
//     // the transformation matrix between basis c1 and c2
// }
