#include "help.h"
#include "para.h"

void para_help_ini( config_t* cfg, parameters* ps )
{
    ps->help = new para_help;
}

void para_help_del( parameters* ps )
{
    delete ps->help;
}
