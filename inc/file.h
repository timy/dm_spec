#ifndef FILE_H_
#define FILE_H_

#include "para.h"
void para_file_ini( struct config_t* cfg, struct parameters *para );
void para_file_del( struct parameters *para );
void set_para_file_one( para_file::file_type type, const char* name,
                    struct parameters *ps );
void set_para_file_mul( para_file::file_type type, const char* name,
                        int n_file, struct parameters *ps );

void open_para_file_read( para_file::file_type type, char* prefix,
                          struct parameters *ps,
                          int n_idx, int *idx );
void open_para_file_write( para_file::file_type type, char* prefix,
                           struct parameters *ps,
                           int n_idx, int *idx );
void close_para_file( para_file::file_type type,
                      struct parameters *ps );

void dm_fprintf( FILE* file, int d );
void dm_fprintf( FILE* file, long d );
void dm_fprintf( FILE* file, double d );
void dm_fprintf( FILE* file, const char* d );

#endif
