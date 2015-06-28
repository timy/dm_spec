#include "para.h"
#include "bath.h"
#include "output.h"
#include "complex.h"
#include <cmath>
#include "debug.h"

#define complex std::complex<double>

void para_bath_config( struct config_t* cfg, struct parameters *ps );
void para_bath_const_redfield_file_config( struct config_t* cfg, struct parameters *ps );
void para_bath_const_redfield_calc_config( struct config_t* cfg, struct parameters *ps );
void para_bath_const_dimer_config( struct config_t* cfg, struct parameters *ps );

#include <libconfig.h>
void para_bath_ini( config_t* cfg, parameters *ps )
{
  ps->bath = new para_bath;
  para_bath_config( cfg, ps );

  ps->bath->redfield_tensor = new complex*** [ps->n_lvl];
  for (int m = 0; m < ps->n_lvl; m ++) {
    ps->bath->redfield_tensor[m] = new complex** [ps->n_lvl];
    for (int n = 0; n < ps->n_lvl; n ++) {
      ps->bath->redfield_tensor[m][n] = new complex* [ps->n_lvl];
      for (int k = 0; k < ps->n_lvl; k ++) {
        ps->bath->redfield_tensor[m][n][k] = new complex [ps->n_lvl];
      }
    }
  }

  if (ps->bath->type == para_bath::CONST_REDFIELD_FILE) {
    para_bath_const_redfield_file_config( cfg, ps );
  } else if (ps->bath->type == para_bath::CONST_REDFIELD_CALC) {
    para_bath_const_redfield_calc_config( cfg, ps );
  } else if (ps->bath->type == para_bath::CONST_DIMER) {
    para_bath_const_dimer_config( cfg, ps );
  }

  set_para_bath( ps );
  //output_redfield_tensor( stdout, ps );
}

void para_bath_del( parameters *ps )
{
  for (int m = 0; m < ps->n_lvl; m ++) {
    for (int n = 0; n < ps->n_lvl; n ++) {
      for (int k = 0; k < ps->n_lvl; k ++) {
        delete[] ps->bath->redfield_tensor[m][n][k];
      }
      delete[] ps->bath->redfield_tensor[m][n];
    }
    delete[] ps->bath->redfield_tensor[m];
  }
  delete[] ps->bath->redfield_tensor;

  delete ps->bath;
}

void para_bath_config( struct config_t* cfg, struct parameters* ps )
{
  const char* name = NULL;
  config_lookup_string( cfg, "bath.type", &name );
  if (strcmp(name, "const_redfield_file") == 0) {
    ps->bath->type = para_bath::CONST_REDFIELD_FILE;
  } else if (strcmp(name, "const_redfield_calc") == 0) {
    ps->bath->type = para_bath::CONST_REDFIELD_CALC;
  } else if (strcmp(name, "const_dimer") == 0) {
    ps->bath->type = para_bath::CONST_DIMER;
  } else {
    fprintf( stderr, "bath.type '%s' unknown. Check "
             "section 'bath' in your configure file\n", name );
  }
}

void bath_set_const_redfield_file( parameters* ps );
void bath_set_const_redfield_calc( parameters* ps );
void bath_set_const_dimer( parameters *ps );

void set_para_bath( parameters *ps )
{
  if (ps->bath->type == para_bath::CONST_REDFIELD_FILE) {
    bath_set_const_redfield_file( ps );
  } else if (ps->bath->type == para_bath::CONST_REDFIELD_CALC) {
    bath_set_const_redfield_calc( ps );
  } else if (ps->bath->type == para_bath::CONST_DIMER) {
    bath_set_const_dimer( ps );
  }
}
