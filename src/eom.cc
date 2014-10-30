#include "eom.h"
#include "para.h"

#include "complex.h"
#include "debug.h"

void equation_of_motion_ini( para_eom* peom )
{
    parameters *ps = peom->ps;
    peom->Rr = new double* [ps->n_lvl];
    peom->Ir = new double* [ps->n_lvl];
    peom->w  = new double* [ps->n_lvl];
    peom->V  = new double* [ps->n_lvl];
    peom->ef = new double [ps->n_pulse];

    peom->y  = new double [ps->n_eom];

    for (int i = 0; i < ps->n_lvl; i ++) {
        peom->Rr[i] = new double [i+1];
        peom->Ir[i] = new double [i+1]; // more than actual use by 1
        peom->w[i] = new double [i+1];  // more than actual use by 1
        peom->V[i] = new double [i+1];  // more than actual use by 1
    }
    // freq diff
    for (int i = 1; i < ps->n_lvl; i ++)
        for (int j = 0; j < i; j ++)
            peom->w[i][j] = ps->energy->energy[i] - ps->energy->energy[j];

    // dissipations
    peom->R  = new double*** [ps->n_lvl];
    for (int m = 0; m < ps->n_lvl; m ++) {
        peom->R[m] = new double** [ps->n_lvl];
        for (int n = 0; n < ps->n_lvl; n ++) {
            peom->R[m][n] = new double* [ps->n_lvl];
            for (int k = 0; k < ps->n_lvl; k ++) {
                peom->R[m][n][k] = new double [ps->n_lvl];
            }
        }
    }
    for (int m = 0; m < ps->n_lvl; m ++) {
        for (int n = 0; n < ps->n_lvl; n ++) {
            for (int k = 0; k < ps->n_lvl; k ++) {
                for (int l = 0; l < ps->n_lvl; l ++)
                    peom->R[m][n][k][l] = real( ps->bath->redfield_tensor[m][n][k][l] );
            }
        }
    }

    // y
    peom->y[0] = 1.0;
    for (int i = 1; i < ps->n_eom; i ++)
        peom->y[i] = 0.0;
}

void equation_of_motion_del( para_eom* peom )
{
    parameters *ps = peom->ps;
    for (int m = 0; m < ps->n_lvl; m ++) {
        for (int n = 0; n < ps->n_lvl; n ++) {
            for (int k = 0; k < ps->n_lvl; k ++) {
                delete[] peom->R[m][n][k];
            }
            delete[] peom->R[m][n];
        }
        delete[] peom->R[m];
    }
    delete[] peom->R;

    for (int i = 0; i < ps->n_lvl; i ++) {
        delete[] peom->V[i];  delete[] peom->w[i];
        delete[] peom->Ir[i]; delete[] peom->Rr[i];
    }
    delete[] peom->y;
    delete[] peom->ef;
    delete[] peom->V; delete[] peom->w; delete[] peom->Ir; delete[] peom->Rr;
}

void eom_output_dm( int it, para_eom* peom );
void eom_output_ef( int it, para_eom* peom );

void equation_of_motion_out( int it, para_eom* peom )
{
    for (int ie = 0; ie < peom->ps->n_eom; ie ++)
        peom->rho[it][ie] = peom->y[ie];
#ifndef USE_MPI
    eom_output_dm( it, peom );
    eom_output_ef( it, peom );
#endif
}

#include <cstdio>
#include "field.h"
void eom_output_dm( int it, para_eom* peom )
{
    FILE* f_dm = peom->ps->file->item[para_file::DM]->f[0]->fptr;
    for (int ie = 0; ie < peom->ps->n_eom; ie ++)
        fprintf( f_dm, "%le ", peom->rho[it][ie] );
    fprintf( f_dm, "\n" );
}

void eom_output_ef( int it, para_eom* peom )
{
    FILE* f_ef = peom->ps->file->item[para_file::EF]->f[0]->fptr;
    double* ef = new double [peom->ps->n_pulse];
    field_real_components( ef, peom->ps->time[it], peom->ps );
    for (int i_dim = 0; i_dim < peom->ps->n_dim; i_dim ++)
        fprintf( f_ef, "%le ", ef[i_dim] /*/ peom->ps->ef[0]->E0*/ );
    fprintf( f_ef, "\n" );
    delete[] ef;
}
