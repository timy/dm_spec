#include "file.h"
#include "para.h"
#include "debug.h"
#include <cstdio>
#include <cstring>

#define MAX_NUMBER_FILE_VAR     8
#define MAX_LENGTH_FILE_LABEL   16
#define MAX_LENGTH_FILE_NAME    256

void para_file_config( struct config_t* cfg, struct parameters* ps );
void genFileNameSyn( para_file::file_item* item );

void para_file_ini( struct config_t* cfg, struct parameters *ps )
{
    ps->file = new para_file;
    para_file_config( cfg, ps );
}

void para_file_del( struct parameters *ps )
{
    para_file *f = ps->file;
    std::map<para_file::file_type, para_file::file_item*>::iterator iter;
    for (iter = f->item.begin(); iter != f->item.end(); ++ iter) {
        para_file::file_item* item = iter->second;
        if (item != NULL) {
            for (int iPtr = 0; iPtr < item->nPtrSyn; iPtr ++) {
#ifndef USE_MPI
                printf( "delete file.item.f.name: %s\n", item->f[iPtr]->name );
#endif
                delete[] item->f[iPtr]->name;
                delete item->f[iPtr];
            }
            delete[] item->f;
            for (int iVar = 0; iVar < item->nVarSyn; iVar ++)
                delete[] item->lbVarSyn[iVar];
            delete[] item->lbVarSyn;
            delete[] item->szVarSyn;
            delete[] item->name;
            delete item;
        }
    }
    delete ps->file;
}

#include <libconfig.h>
void para_file_config( struct config_t* cfg, struct parameters* ps )
{
    // temporary variables
    const char* name = NULL;
    int* szVarSyn = new int [MAX_NUMBER_FILE_VAR];
    char** lbVarSyn = new char* [MAX_NUMBER_FILE_VAR];
    for (int iVar = 0; iVar < MAX_NUMBER_FILE_VAR; iVar ++)
        lbVarSyn[iVar] = new char [MAX_LENGTH_FILE_LABEL];

    config_lookup_string( cfg, "file.dm", &name );
    set_para_file( para_file::DM, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.ef", &name );
    set_para_file( para_file::EF, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.time", &name );
    set_para_file( para_file::TIME, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.info", &name );
    set_para_file( para_file::INFO, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.grid_1d", &name );
    set_para_file( para_file::GRID_1D, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.ppar_1d", &name );
    szVarSyn[0] = ps->n_dpl; strcpy( lbVarSyn[0], "dpl" );
    szVarSyn[1] = ps->n_dim; strcpy( lbVarSyn[1], "coo" );
    set_para_file( para_file::PPAR_1D, name, 2, szVarSyn, lbVarSyn, ps );

    config_lookup_string( cfg, "file.ptot_1d", &name );
    szVarSyn[0] = ps->n_dpl; strcpy( lbVarSyn[0], "dpl" );
    szVarSyn[1] = ps->n_dim; strcpy( lbVarSyn[1], "coo" );
    set_para_file( para_file::PTOT_1D, name, 2, szVarSyn, lbVarSyn, ps );

    config_lookup_string( cfg, "file.grid_2d", &name );
    set_para_file( para_file::GRID_2D, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.ppar_2d", &name );
    szVarSyn[0] = ps->n_dpl; strcpy( lbVarSyn[0], "dpl" );
    szVarSyn[1] = ps->n_dim; strcpy( lbVarSyn[1], "coo" );
    set_para_file( para_file::PPAR_2D, name, 2, szVarSyn, lbVarSyn, ps );

    config_lookup_string( cfg, "file.ptot_2d", &name );
    szVarSyn[0] = ps->n_dpl; strcpy( lbVarSyn[0], "dpl" );
    szVarSyn[1] = ps->n_dim; strcpy( lbVarSyn[1], "coo" );
    set_para_file( para_file::PTOT_2D, name, 2, szVarSyn, lbVarSyn, ps );

    config_lookup_string( cfg, "file.orient", &name );
    set_para_file( para_file::ORIENT, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.config", &name );
    set_para_file( para_file::CONFIG, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.kuvl", &name );
    set_para_file( para_file::KUVL, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.kuvm", &name );
    set_para_file( para_file::KUVM, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.kl", &name );
    set_para_file( para_file::KL, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.km", &name );
    set_para_file( para_file::KM, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.euvl", &name );
    set_para_file( para_file::EUVL, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.euvm", &name );
    set_para_file( para_file::EUVM, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.rl", &name );
    set_para_file( para_file::RL, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.rm", &name );
    set_para_file( para_file::RM, name, 0, NULL, NULL, ps );

    config_lookup_string( cfg, "file.log", &name );
    set_para_file( para_file::LOG, name, 0, NULL, NULL, ps );

    // delete temporary variables
    for (int iVar = 0; iVar < MAX_NUMBER_FILE_VAR; iVar ++)
        delete[] lbVarSyn[iVar];
    delete[] lbVarSyn;
    delete[] szVarSyn;
}

void set_para_file( para_file::file_type type, const char* name,
                    int nVar, int* szVar, char** lbVar,
                    parameters* ps )
{
    if (ps->file->item[type] != NULL) {
        fprintf( stderr, "Type %d (%s) can only be set once.\n", type, name );
        return;
    }
    para_file::file_item* item = ps->file->item[type] = new para_file::file_item;
    // name
    item->name = new char [MAX_LENGTH_FILE_NAME];
    strcpy( item->name, name );
    // nVarSyn
    item->nVarSyn = nVar;
    // szVarSyn
    item->szVarSyn = new int [nVar];
    for (int iVar = 0; iVar < nVar; iVar ++)
        item->szVarSyn[iVar] = szVar[iVar];
    // lbVarSyn
    item->lbVarSyn = new char* [nVar];
    for (int iVar = 0; iVar < nVar; iVar ++) {
        item->lbVarSyn[iVar] = new char [MAX_LENGTH_FILE_LABEL];
        strcpy( item->lbVarSyn[iVar], lbVar[iVar] );
    }
    // nPtrSyn
    int nPtrSyn = 1;
    for (int iVar = 0; iVar < nVar; iVar ++)
        nPtrSyn *= szVar[iVar];
    item->nPtrSyn = nPtrSyn;
    // node f
    item->f = new para_file::file_item::node* [nPtrSyn];
    for (int iPtr = 0; iPtr < nPtrSyn; iPtr ++) {
        item->f[iPtr] = new para_file::file_item::node;
        item->f[iPtr]->name = new char [MAX_LENGTH_FILE_NAME];
        item->f[iPtr]->fptr = NULL;
    }
    genFileNameSyn( item );
}

void fmtFileNameSyn( int count, int* idx,  para_file::file_item* item )
{
    strcpy( item->f[count]->name, item->name );
    for (int iVar = 0; iVar < item->nVarSyn; iVar ++) {
        sprintf( item->f[count]->name, "%s_%s%d",
                 item->f[count]->name, item->lbVarSyn[iVar], idx[iVar] );
    }
}

void walk( int lvl, int nLvl, int* size, int* idx, int* count,
           void (*func)( int, int*, para_file::file_item* ),
           para_file::file_item* item )
{
    if (lvl == nLvl) {
        func( *count, idx, item );
        (*count) ++;
        return;
    } else {
        for (int i = 0; i < size[lvl]; i ++) {
            idx[lvl] = i;
            walk( lvl+1, nLvl, size, idx, count, func, item );
        }
    }
}

void genFileNameSyn( para_file::file_item* item )
{
    int* idx = new int [item->nVarSyn];
    int count = 0;
    walk( 0, item->nVarSyn, item->szVarSyn, idx, &count, fmtFileNameSyn, item );
    delete[] idx;
}

void genFileNameAsy( para_file::file_item* item, char* prefix,
                     int iPtr, int nIdx, char** lb, int* idx, char* fileName )
{
    if (prefix == NULL) {
        strcpy( fileName, item->f[iPtr]->name );
    } else {
        strcpy( fileName, prefix );
        strcat( fileName, item->f[iPtr]->name );
    }
    for (int i = 0; i < nIdx; i ++) {
        if (lb == NULL)
            sprintf( fileName, "%s_%d", fileName, idx[i] );
        else
            sprintf( fileName, "%s_%s%d", fileName, lb[i], idx[i] );
    }
    strcat( fileName, ".dat" );
}

void open_para_file( para_file::file_type type, char* prefix, parameters* ps,
                     int nIdx, char** lb, int *idx, const char* rw )
{
    char fileName[MAX_LENGTH_FILE_NAME];
    para_file::file_item* item = ps->file->item[type];
    for (int iPtr = 0; iPtr < item->nPtrSyn; iPtr ++) {
        genFileNameAsy( item, prefix, iPtr, nIdx, lb, idx, fileName );
        item->f[iPtr]->fptr = fopen( fileName, rw );
        if (item->f[iPtr]->fptr == NULL)
            fprintf( stderr, "Cannot open file %s\n", fileName );
    }
}

void close_para_file( para_file::file_type type, parameters* ps )
{
    para_file::file_item* item = ps->file->item[type];
    for (int iPtr = 0; iPtr < item->nPtrSyn; iPtr ++) {
        if (item->f[iPtr]->fptr != NULL)
            fclose( item->f[iPtr]->fptr );
    }
}

FILE* get_fptr_from_idxSyn( para_file::file_type type, int* idx, parameters* ps )
{
    para_file::file_item* item = ps->file->item[type];
    int index = 0; int factor = 1;
    for (int iVar = 0; iVar < item->nVarSyn; iVar ++) {
        int id = item->nVarSyn - 1 - iVar;
        index += factor * idx[id];
        factor *= item->szVarSyn[id];
    }
    return item->f[index]->fptr;
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
