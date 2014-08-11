#ifndef CFG_H_
#define CFG_H_

typedef struct _IO_FILE FILE;

int read_cfg_file( FILE *file, void (*f_proc) ( char*, char*, void* ), void *p );

#endif
