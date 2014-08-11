#include <cstdio>
#include <cstring>
#include "mpi_cfg.h"
#include "cfg.h"

void proc_para( char *key_buf, char *val_buf, void *p )
{
    mpi_configure *par = (mpi_configure*) p;
    if ( strcmp( key_buf, "n_task_per_cpu" ) == 0 )
        sscanf( val_buf, "%ld", &(par->n_task_per_cpu) );
    return;
}

void read_mpi_cfg( mpi_configure *cfg )
{
    FILE *file = fopen( "cfg/mpi.cfg", "r" );
    read_cfg_file( file, proc_para, (void*)cfg );
    fclose( file );
}
