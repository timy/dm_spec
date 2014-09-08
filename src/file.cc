#include "file.h"
#include "para.h"
#include "debug.h"
#include <cstdio>
#include <cstring>


void para_file_config( struct config_t* cfg, struct parameters* ps );

void para_file_ini( struct config_t* cfg, struct parameters *ps )
{
    ps->file = new para_file;
    para_file_config( cfg, ps );
}

void para_file_del( struct parameters *ps )
{
    para_file *f = ps->file;
    // loop over all file.one_node
    std::map<para_file::file_type, para_file::one_node*>::iterator it1;
    for (it1 = f->one.begin(); it1 != f->one.end(); ++ it1) {
        if (it1->second != NULL) {
            // file.one_node.name
            if (it1->second->name != NULL) {
#ifndef USE_MPI
                printf( "delete file.one_node.name: %s\n",
                        it1->second->name );
#endif
                delete[] it1->second->name;
            }
            it1->second->name = NULL;
            // file.one_node.fptr
            it1->second->fptr = NULL;
            // file.one_node
            delete it1->second;
        }
    }

    // loop over all file.mul_node
    std::map<para_file::file_type, para_file::mul_node*>::iterator it2;
    for (it2 = f->mul.begin(); it2 != f->mul.end(); ++ it2) {
        if (it2->second != NULL) {
            int n_file = it2->second->n_file;
            // file.mul_node.name
            for (int i_file = 0; i_file < n_file; i_file ++) {
#ifndef USE_MPI
                printf( "delete file.mul_node.name: %s\n",
                        it2->second->name[i_file] );
#endif
                delete[] it2->second->name[i_file];
            }
            delete[] it2->second->name;
            // file.mul_node.fptr
            if (it2->second->fptr != NULL)
                delete[] it2->second->fptr;
            // file.mul_node
            delete it2->second;
        }
    }

    // file
    delete ps->file;
}

#include <libconfig.h>
void para_file_config( struct config_t* cfg, struct parameters* ps )
{
    const char* name = NULL;
    config_lookup_string( cfg, "file.dm", &name );
    set_para_file_one( para_file::DM, name, ps );
    config_lookup_string( cfg, "file.ef", &name );
    set_para_file_one( para_file::EF, name, ps );
    config_lookup_string( cfg, "file.time", &name );
    set_para_file_one( para_file::TIME, name, ps );
    config_lookup_string( cfg, "file.info", &name );
    set_para_file_one( para_file::INFO, name, ps );
    config_lookup_string( cfg, "file.grid_1d", &name );
    set_para_file_one( para_file::GRID_1D, name, ps );
    config_lookup_string( cfg, "file.ppar_1d", &name );
    set_para_file_mul( para_file::PPAR_1D, name, (ps->n_dpl*ps->n_dim), ps );
    config_lookup_string( cfg, "file.ptot_1d", &name );
    set_para_file_mul( para_file::PTOT_1D, name, (ps->n_dpl*ps->n_dim), ps );
    config_lookup_string( cfg, "file.grid_2d", &name );
    set_para_file_one( para_file::GRID_2D, name, ps );
    config_lookup_string( cfg, "file.ppar_2d", &name );
    set_para_file_mul( para_file::PPAR_2D, name, (ps->n_dpl*ps->n_dim), ps );
    config_lookup_string( cfg, "file.ptot_2d", &name );
    set_para_file_mul( para_file::PTOT_2D, name, (ps->n_dpl*ps->n_dim), ps );
    config_lookup_string( cfg, "file.orient", &name );
    set_para_file_one( para_file::ORIENT, name, ps );
    config_lookup_string( cfg, "file.config", &name );
    set_para_file_one( para_file::CONFIG, name, ps );
    config_lookup_string( cfg, "file.kuvl", &name );
    set_para_file_one( para_file::KUVL, name, ps );
    config_lookup_string( cfg, "file.kuvm", &name );
    set_para_file_one( para_file::KUVM, name, ps );
    config_lookup_string( cfg, "file.kl", &name );
    set_para_file_one( para_file::KL, name, ps );
    config_lookup_string( cfg, "file.km", &name );
    set_para_file_one( para_file::KM, name, ps );
    config_lookup_string( cfg, "file.euvl", &name );
    set_para_file_one( para_file::EUVL, name, ps );
    config_lookup_string( cfg, "file.euvm", &name );
    set_para_file_one( para_file::EUVM, name, ps );
    config_lookup_string( cfg, "file.rl", &name );
    set_para_file_one( para_file::RL, name, ps );
    config_lookup_string( cfg, "file.rm", &name );
    set_para_file_one( para_file::RM, name, ps );
    config_lookup_string( cfg, "file.log", &name );
    set_para_file_one( para_file::LOG, name, ps );
}

void set_para_file_one( para_file::file_type type, const char* name,
                        struct parameters *ps)
{
    para_file *f = ps->file;
    // file.one_node
    if (f->one[type] == NULL) {
        f->one[type] = new para_file::one_node;
        f->one[type]->n_file = 1;
        // file.one_node.name
        f->one[type]->name = new char[256];
        strcpy( f->one[type]->name, name );
        // file.one_node.fptr
        f->one[type]->fptr = NULL;
#ifndef USE_MPI
        printf( "create file.one_node: %s\n", f->one[type]->name );
#endif
        f->mul[type] = NULL;
    }
}

void set_para_file_mul( para_file::file_type type, const char* name,
                        int n_file, struct parameters *ps )
{
    para_file *f = ps->file;
    if (f->mul[type] == NULL) {
        f->mul[type] = new para_file::mul_node;
        f->mul[type]->n_file = n_file;
        // file.mul_node.name
        f->mul[type]->name = new char*[n_file];
        for (int i_file = 0; i_file < n_file; i_file ++) {
            f->mul[type]->name[i_file] = new char[256];
            sprintf( f->mul[type]->name[i_file], "%s%d", name, i_file );
        }
        // file.mul_node.fptr
        f->mul[type]->fptr = new FILE*[n_file];
        for (int i_file = 0; i_file < n_file; i_file ++)
            f->mul[type]->fptr[i_file] = NULL;
#ifndef USE_MPI
        for (int i_file = 0; i_file < n_file; i_file ++)
            printf( "create file file.mul_node: %s\n", f->mul[type]->name[i_file] );
#endif
        f->one[type] = NULL;
    }
}

void file_name_generate( char* name_basic, char* file_name, int n_idx, int *idx );
void open_para_file_read( para_file::file_type type, char *prefix,
                          struct parameters *ps, int n_idx, int *idx )
{
    char file_name[512], sub_name[256];
    para_file::one_node* f1 = ps->file->one[type];
    para_file::mul_node* f2 = ps->file->mul[type];
    if (f1 != NULL && f2 == NULL) {
        file_name_generate( f1->name, sub_name, n_idx, idx );
        if (prefix == NULL)
            strcpy( file_name, sub_name );
        else {
            strcpy( file_name, prefix );
            strcat( file_name, sub_name );
        }
        f1->fptr = fopen( file_name, "r" );
        if (f1->fptr == NULL)
            error( ps, "%s", file_name );
    }
    else if (f2 != NULL && f1 == NULL) {
        for (int i_file = 0; i_file < f2->n_file; i_file ++) {
            file_name_generate( f2->name[i_file], sub_name, n_idx, idx );
            if (prefix == NULL)
                strcpy( file_name, sub_name );
            else {
                strcpy( file_name, prefix );
                strcat( file_name, sub_name );
            }
            f2->fptr[i_file] = fopen( file_name, "r" );
            if (f2->fptr[i_file] == NULL)
                error( ps, "%s", file_name );
        }
    }
    else {
        error( ps, "%s", "something is wrong with files..." );
    }

}
void open_para_file_write( para_file::file_type type, char *prefix,
                           struct parameters *ps, int n_idx, int *idx )
{
    char file_name[512], sub_name[256];
    para_file::one_node* f1 = ps->file->one[type];
    para_file::mul_node* f2 = ps->file->mul[type];
    if (f1 != NULL && f2 == NULL) {
        file_name_generate( f1->name, sub_name, n_idx, idx );
        if (prefix == NULL)
            strcpy( file_name, sub_name );
        else {
            strcpy( file_name, prefix );
            strcat( file_name, sub_name );
        }
        f1->fptr = fopen( file_name, "w" );
        if (f1->fptr == NULL)
            error( ps, "%s", file_name );
    }
    else if (f2 != NULL && f1 == NULL) {
        for (int i_file = 0; i_file < f2->n_file; i_file ++) {
            file_name_generate( f2->name[i_file], sub_name, n_idx, idx );
            if (prefix == NULL)
                strcpy( file_name, sub_name );
            else {
                strcpy( file_name, prefix );
                strcat( file_name, sub_name );
            }
            f2->fptr[i_file] = fopen( file_name, "w" );
            if (f2->fptr[i_file] == NULL)
                error( ps, "%s", file_name );
        }
    }
    else {
        error( ps, "%s", "something is wrong with files..." );
    }
}

void file_name_generate( char* name_basic, char* file_name, int n_idx, int *idx )
{
    char str_tmp[64];
    // para_file::one_node *f = ps->file->one[type];

    // e.g.: file_name = "res/dm"
    strcpy( file_name, name_basic );

    // e.g.: file_name = "res/dm_ 2_16"
    for (int i = 0; i < n_idx; i ++) {
        sprintf( str_tmp, "_%d", idx[i] );
        strcat( file_name, str_tmp );
    }
    // e.g.: file_name = "res/dm_ 2_16.dat"
    strcat( file_name, ".dat" );
}

void close_para_file( para_file::file_type type, struct parameters *ps )
{
    para_file::one_node* f1 = ps->file->one[type];
    para_file::mul_node* f2 = ps->file->mul[type];
    if (f1 != NULL && f2 == NULL) {
        if (f1->fptr != NULL)
            fclose( f1->fptr );
    }
    else if (f2 != NULL && f1 == NULL) {
        for (int i_file = 0; i_file < f2->n_file; i_file ++)
            if (f2->fptr[i_file] != NULL)
                fclose( f2->fptr[i_file] );
    }
    else {
        error( ps, "%s", "something is wrong with files..." );
    }
}


void dm_fprintf( FILE* file, int d )
{
    fprintf( file, "%d", d );
}

void dm_fprintf( FILE* file, long d )
{
    fprintf( file, "%ld", d );
}

void dm_fprintf( FILE* file, double d )
{
    fprintf( file, "%le", d );
}

void dm_fprintf( FILE* file, const char* d )
{
    fprintf( file, "%s", d );
}
