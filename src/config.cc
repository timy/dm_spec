#include "config.h"
#include "debug.h"
#include "libconfig.h"

void config_file_ini( config_t *cfg, const char* file_name )
{
    config_init( cfg );
    if ( config_read_file( cfg, file_name ) == CONFIG_FALSE ) {
        print( config_error_file( cfg ) );
        print( config_error_line( cfg ) );
        print( config_error_text( cfg ) );        
        config_destroy( cfg );
        error_at( "config_file_ini" );
    }
}

void config_file_del( config_t *cfg )
{
    config_destroy( cfg );
}
