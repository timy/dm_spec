////////////////////////////////////////////////////////////
// Calculation of single molecules for 1D spectroscopy
////////////////////////////////////////////////////////////

#include "para.h"
#include "seidner.h"
#include "polar.h"
#include "output.h"
#include "util.h"
#include "spec.h"
#include "file.h"
#include "eom.h"
#include <complex>
#include <cstdio>

#define complex std::complex<double>
#include <cstdlib>
#include "debug.h"
int main( int argc, char* argv[] )
{
    tic();

    parameters ps = { .f_eom = equation_of_motion_full };
    para_ini( &ps, "cfg/parameters.cfg" );

    // ppar_1d, ptot_1d: n_phase * nx * n_dim
    complex ***ppar_1d = prepare_pol_array_seidner( 1, &ps );
    complex ***ptot_1d = prepare_pol_array_seidner( 1, &ps );
    
    // calculation
    calc_ptot_seidner( ptot_1d, &ps );
    calc_ppar_seidner( ppar_1d, ptot_1d, &ps );

    // output orientation
    open_para_file_write( para_file::ORIENT, NULL, &ps, 0, NULL );
    output_mol_orient( ps.file->one[para_file::ORIENT]->fptr, &ps );
    close_para_file( para_file::ORIENT, &ps );
    // output kuvL
    open_para_file_write( para_file::KUVL, NULL, &ps, 0, NULL );
    output_ef_kuvL( ps.file->one[para_file::KUVL]->fptr, &ps );
    close_para_file( para_file::KUVL, &ps );
    // output kuvM
    open_para_file_write( para_file::KUVM, NULL, &ps, 0, NULL );
    output_ef_kuvM( ps.file->one[para_file::KUVM]->fptr, &ps );
    close_para_file( para_file::KUVM, &ps );
    // output EuvL
    open_para_file_write( para_file::EUVL, NULL, &ps, 0, NULL );
    output_ef_EuvL( ps.file->one[para_file::EUVL]->fptr, &ps );
    close_para_file( para_file::EUVL, &ps );    
    // output EuvM
    open_para_file_write( para_file::EUVM, NULL, &ps, 0, NULL );
    output_ef_EuvM( ps.file->one[para_file::EUVM]->fptr, &ps );
    close_para_file( para_file::EUVM, &ps );
    // output rL
    open_para_file_write( para_file::RL, NULL, &ps, 0, NULL );
    output_rL_seidner( ps.file->one[para_file::RL]->fptr, &ps );
    close_para_file( para_file::RL, &ps );
    // output rM
    open_para_file_write( para_file::RM, NULL, &ps, 0, NULL );
    output_rM_seidner( ps.file->one[para_file::RM]->fptr, &ps );
    close_para_file( para_file::RM, &ps );

    // output info
    // open_para_file_write( para_file::INFO, NULL, &ps, 0, NULL );
    // output_info( 0, 0, 0,
    //               ps.file->one[para_file::INFO]->fptr,
    //               &ps );
    // close_para_file( para_file::INFO, &ps );

    // output ppar_1d
    // output_pol_1d( para_file::PPAR_1D, ppar_1d, &ps, 0, NULL );
    for (int i_dir = 0; i_dir < ps.seid->n_phase; i_dir ++) {
        int file_idx[1] = { i_dir };
        open_para_file_write( para_file::PPAR_1D, NULL, &ps, 1, 
                              file_idx );
        io_pol_write( ps.file->mul[para_file::PPAR_1D]->fptr,
                      ppar_1d[i_dir], &ps );
        close_para_file( para_file::PPAR_1D, &ps );
    }
    
    // output ptot_1d
    // output_pol_1d( para_file::PTOT_1D, ptot_1d, &ps, 0, NULL );
    for (int i_phase = 0; i_phase < ps.seid->n_phase; i_phase ++) {
        int file_idx[1] = { i_phase };
        open_para_file_write( para_file::PTOT_1D, NULL, &ps, 1,
                              file_idx );
        io_pol_write( ps.file->mul[para_file::PTOT_1D]->fptr,
                      ptot_1d[i_phase], &ps );
        close_para_file( para_file::PTOT_1D, &ps );
    }

    // output spec_1d
    // spec_1d( ppar_1d, "res/spec_1d", &ps );

    // clean
    clean_pol_array_seidner( 1, ppar_1d, &ps );
    clean_pol_array_seidner( 1, ptot_1d, &ps );

    para_del( &ps );

    tac( stdout );
    printf( "Hasta la vista!\n" );
    
    return 0;
}
