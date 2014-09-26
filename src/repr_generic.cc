#include "para.h"
#include "repr.h"
#include <cstdio>


// TODO: needs rewrite here, or delete it!!
void repr_set_generic( parameters *ps, para_repr_generic *pg )
{
    for (int i_lvl = 0; i_lvl < ps->n_lvl; i_lvl ++)
        ps->energy->energy[i_lvl] = pg->E[i_lvl];

    for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            ps->dipole->dipole[i_dpl][i_dim] = pg->mu[i_dpl][i_dim];
}

#include <libconfig.h>
void cdf_read( const char* name, int count, void (*sscanf_st)(char*, int, parameters*),
              parameters* ps );
void sscanf_st_energy( char* buf, int i, parameters* ps );
void sscanf_st_dipole( char* buf, int i, parameters* ps );

void para_repr_generic_config( config_t *cfg, parameters* ps )
{
    const char* name = NULL;

    config_lookup_string( cfg, "generic.E", &name );
    cdf_read( name, ps->n_lvl, sscanf_st_energy, ps );

    config_lookup_string( cfg, "generic.mu", &name );
    cdf_read( name, ps->n_dpl, sscanf_st_dipole, ps );
}

int is_valid_line( char* ptrBuf )
{
    int i = 0, bValid = 0;
    char c_pre = '\0';
    do {
        char c = ptrBuf[i];
        if (c == '/' && c_pre == '/') {
            bValid = 0; break;
        }
        if (c != '\t' && c != '\r' && c != '\n' && c != ' ' && c != '/') {
            bValid = 1; break;
        }
        c_pre = c;
    } while (ptrBuf[++i] != '\0');
    return bValid;
}

#define NMAX 1024

void file_walk_lines( FILE* file, void (*proc)(char*, int, void* st), void* st ) {
    char* buffer = new char [NMAX];
    char* ptrBuf = NULL;
    int count = 0;
    do {
        ptrBuf = fgets( buffer, NMAX, file );
        if (ptrBuf != NULL) {
            if (is_valid_line( ptrBuf ) == 1) {
                proc( ptrBuf, count, st );
                count ++;
            }
        }
        else // EOF
            break;
    } while (1);
    delete[] buffer;
    rewind( file );
}

// count lines
struct st_file_count_lines { int nLine; };
void proc_file_count_lines(char* ptrBuf, int count, void* st) {
    ((st_file_count_lines*) st)->nLine ++;
}
int file_count_lines( FILE* file ) {
    st_file_count_lines st;
    st.nLine = 0;
    file_walk_lines( file, proc_file_count_lines, (void*)&st );
    return st.nLine;
}

struct st_cdf_read_driver {
    void (*sscanf_st)( char* ptrBuf, int count, parameters* ps );
    parameters* ps;
};
void proc_cdf_read_driver(char* ptrBuf, int count, void* st) {
    parameters *ps = ((st_cdf_read_driver*) st)->ps;
    ((st_cdf_read_driver*) st)->sscanf_st( ptrBuf, count, ps );
}
void cdf_read_driver( FILE* file, void (*sscanf_st)( char*, int, parameters* ),
                      parameters* ps ) {
    st_cdf_read_driver st;
    st.sscanf_st = sscanf_st; st.ps = ps;
    file_walk_lines( file, proc_cdf_read_driver, (void*)&st );
}

void cdf_read( const char* name, int count, void (*sscanf_st)( char*, int, parameters* ),
              parameters* ps )
{
    FILE* file = NULL;
    if (NULL != (file = fopen( name, "r" )))  {
        int nLine = file_count_lines( file );
        if (count != nLine)
            fprintf( stderr, "CDF warning: no. of lines (%d) in file %s does not agree with "
                     "no. of data (%d) defined in configuration file.\n",
                     nLine, name, count );
        cdf_read_driver( file, sscanf_st, ps );
        fclose( file );
    } else {
        fprintf( stderr, "CDF error: Cannot open file %s", name );
    }
}

void sscanf_st_energy( char* buf, int i, parameters* ps ) {
    int* index = ps->energy->index;
    double* energy = ps->energy->energy;
    int nCol = sscanf( buf, "%d %lf", &(index[i]), &(energy[i]) );
    energy[i] *= C_cm2au;
    if (nCol != 2)
        fprintf( stderr, "CDF warning: suspicous ENERGY data format, nCol=%d\n", nCol );
}

void sscanf_st_dipole( char* buf, int i, parameters* ps ) {
    int** index = ps->dipole->index;
    double** dipole = ps->dipole->dipole;
    int nCol = sscanf( buf, "%d %d %lf %lf %lf", &(index[i][0]), &(index[i][1]),
                       &(dipole[i][0]), &(dipole[i][1]), &(dipole[i][2]) );
    if (nCol != 5)
        fprintf( stderr, "CDF warning: suspicous DIPOLE data format, nCol=%d\n", nCol );
}
