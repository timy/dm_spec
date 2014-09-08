#ifndef FILE_H_
#define FILE_H_

#include "para.h"
void para_file_ini( struct config_t* cfg, struct parameters *para );
void para_file_del( struct parameters *para );
void set_para_file( para_file::file_type type, const char* name,
                    int nVarSyn, int* szVarSyn, char** lbVarSyn,
                    struct parameters *ps );

void open_para_file( para_file::file_type type, char *prefix, parameters* ps,
                     int nIdx, char** lb, int *idx, const char* rw );
void close_para_file( para_file::file_type type, parameters* ps );

typedef struct _IO_FILE FILE;
FILE* get_fptr_from_idxSyn( para_file::file_type type, int* idx, parameters* ps );

void dm_fprintf( FILE* file, int d );
void dm_fprintf( FILE* file, long d );
void dm_fprintf( FILE* file, double d );
void dm_fprintf( FILE* file, const char* d );

#endif
