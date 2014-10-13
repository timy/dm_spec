////////////////////////////////////////////////////////////
// Calculation of single molecules for 1D spectroscopy with Seidner's method
////////////////////////////////////////////////////////////

#include "para.h"
#include "seidner.h"
#include "polar.h"
#include "output.h"
#include "util.h"
#include "file.h"
#include "eom.h"
#include "postproc.h"
#include <complex>
#include <cstdio>

#define complex std::complex<double>
#include <cstdlib>
#include "debug.h"
int main( int argc, char* argv[] )
{
    tic();

    parameters ps;
    const char dirBase[] = "./";
    para_ini( &ps, dirBase );

    if (ps.pols->method_ppar != para_pols::SEIDNER) {
        fprintf( stderr, "You need to set pols.ppar_calc_method"
                 " to be 'seidner' for this program.\n" );
        para_del( &ps ); exit( -1 );
    }

    if (ps.mvar->ny > 1) {
        fprintf( stderr, "You need to set ny = 1 for this program.\n" );
        para_del( &ps );
        exit( -1 );
    }

    // ppar_1d, ptot_1d: n_phase * nx * n_dpl* n_dim
    complex ****ppar_1d = prepare_pol_array_seidner( 1, &ps );
    complex ****ptot_1d = prepare_pol_array_seidner( 1, &ps );

    // calculation
    calc_ptot_seidner( ptot_1d, &ps );
    calc_ppar_seidner( ppar_1d, ptot_1d, &ps );

    // output orientation
    open_para_file( para_file::ORIENT, NULL, &ps, 0, NULL, NULL, "w" );
    output_mol_orient( ps.file->item[para_file::ORIENT]->f[0]->fptr, &ps );
    close_para_file( para_file::ORIENT, &ps );
    // output kuvL
    open_para_file( para_file::KUVL, NULL, &ps, 0, NULL, NULL, "w" );
    output_ef_kuvL( ps.file->item[para_file::KUVL]->f[0]->fptr, &ps );
    close_para_file( para_file::KUVL, &ps );
    // output kuvM
    open_para_file( para_file::KUVM, NULL, &ps, 0, NULL, NULL, "w" );
    output_ef_kuvM( ps.file->item[para_file::KUVM]->f[0]->fptr, &ps );
    close_para_file( para_file::KUVM, &ps );
    // output EuvL
    open_para_file( para_file::EUVL, NULL, &ps, 0, NULL, NULL, "w" );
    output_ef_EuvL( ps.file->item[para_file::EUVL]->f[0]->fptr, &ps );
    close_para_file( para_file::EUVL, &ps );
    // output EuvM
    open_para_file( para_file::EUVM, NULL, &ps, 0, NULL, NULL, "w" );
    output_ef_EuvM( ps.file->item[para_file::EUVM]->f[0]->fptr, &ps );
    close_para_file( para_file::EUVM, &ps );
    // output rL
    open_para_file( para_file::RL, NULL, &ps, 0, NULL, NULL, "w" );
    output_rL_seidner( ps.file->item[para_file::RL]->f[0]->fptr, &ps );
    close_para_file( para_file::RL, &ps );
    // output rM
    open_para_file( para_file::RM, NULL, &ps, 0, NULL, NULL, "w" );
    output_rM_seidner( ps.file->item[para_file::RM]->f[0]->fptr, &ps );
    close_para_file( para_file::RM, &ps );

    io_pol_dir_write( para_file::PPAR_1D, ppar_1d, ps.seid->n_phase, NULL, &ps );
    io_pol_dir_write( para_file::PTOT_1D, ptot_1d, ps.seid->n_phase, NULL, &ps );

    // clean
    clean_pol_array_seidner( 1, ppar_1d, &ps );
    clean_pol_array_seidner( 1, ptot_1d, &ps );

    para_del( &ps );

    tac( stdout );
    printf( "Hasta la vista!\n" );

    return 0;
}
