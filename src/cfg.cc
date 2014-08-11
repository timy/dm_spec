#include "cfg.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define NMAX 1024

int read_key_and_value(char *buf, char *key, char *val);

int read_cfg_file( FILE *file, void (*f_proc) ( char*, char*, void* ), void *p )
{
    char buf[NMAX], key_buf[NMAX], val_buf[NMAX];
    while (!feof( file )) {
        fgets( buf, NMAX, file );
        int ierr = read_key_and_value( buf, key_buf, val_buf );
        if (ierr == 0)
            f_proc( key_buf, val_buf, p );
    }
    return 0;
}

enum status {
    STAT_PRE_KEY,
    STAT_KEY, 
    STAT_PRE_SET,
    STAT_PRE_VAL,
    STAT_VAL };

int read_key_and_value(char *buf, char *key, char *val)
{
    int i_key, i_val, ierr = -1;
    char *pc = buf;
    status s = STAT_PRE_KEY;
    while (1) {
        char c = *pc ++;
        // printf("char = %d\n", c);
        if (s == STAT_PRE_KEY) {
            if (c == '\t' || c == ' ') {
                // printf("pre_key -> pre_key\n");
                s = STAT_PRE_KEY;
            }
            else if ((c >= 'A' && c <= 'Z') ||
                     (c >= 'a' && c <= 'z')) {
                // printf("pre_key -> key\n");
                i_key = 0;
                key[i_key] = c;
                s = STAT_KEY; 
            }
            else if (c == '#' || c == '\0' || c == '\n') {
                // printf("pre_key -> end\n");
                break;}
            else {
                printf( "Illegal character in STAT_PRE_KEY: %d\n", c );
                exit( -1 );
            }
        }
        else if (s == STAT_KEY) {
            if (c == '\t' || c == ' ') {
                // printf("key -> pre_set\n");
                key[++i_key] = '\0';
                s = STAT_PRE_SET;
            }
            else if (c == '=') {
                // printf("key -> pre_val\n");
                key[++i_key] = '\0';
                s = STAT_PRE_VAL;
            }
            else if ((c >= 'A' && c <= 'Z') ||
                     (c >= 'a' && c <= 'z') ||
                     (c >= '0' && c <= '9') ||
                     c == '_' || c == '-') {
                // printf("key -> key\n");
                key[++i_key] = c; 
                s = STAT_KEY;
            }
            else if (c == '#' || c == '\0' || c == '\n') {
                // printf("key -> end\n");
                break;
            }
            else {
                printf( "Illegal character in STAT_KEY: %c\n", c );
                exit( -1 );
            }
        }
        else if (s == STAT_PRE_SET) {
            if (c == '\t' || c == ' ') {
                // printf("pre_set -> pre_set\n");
                s = STAT_PRE_SET; 
            }
            else if (c == '=') {
                // printf("pre_set -> pre_val\n");
                s = STAT_PRE_VAL; 
            }
            else if (c == '#' || c == '\0' || c == '\n') {
                // printf("pre_set -> end\n");
                break; 
            }
            else {
                printf( "Illegal character in STAT_SET: %c\n", c );
            }
        }
        else if (s == STAT_PRE_VAL) {
            if (c == '\t' || c == ' ') {
                // printf("pre_val -> pre_val\n");
                s = STAT_PRE_VAL;
            }
            else if ((c >= '0' && c <= '9') || c == '.' ||
                     c == '-') {
                // printf("pre_val -> val\n");
                val[i_val = 0] = c;
                s = STAT_VAL;
            }
            else if (c == '#' || c == '\0' || c == '\n') {
                // printf("pre_val -> end\n");
                break; 
            }
            else {
                printf( "Illegal character in STAT_PRE_VAL: %c\n", c );
            }
        }
        else if (s == STAT_VAL) {
            if (c == '\t' || c == ' ' || 
                c == '#' || c == '\0' || c == '\n') {
                // printf("val -> end\n");
                val[++i_val] = '\0';
                ierr = 0; 
                break;
            }
            else if ((c >= '0' && c <= '9') || c == '.' ||
                     c == '-' || c == 'e' || c == 'E') {
                // printf("val -> val\n");
                val[++i_val] = c;
                s = STAT_VAL; 
            }
        }
        else {
            fprintf( stdout, "Unknown status\n" ); exit(-1);
        }
    }
    return ierr;
}
