#include "para.h"
#include "polar.h"
#include "file.h"
#include "output.h"
#include "complex.h"
#include "debug.h"
#include "postproc.h"

int flag = 0; // for debug

#define N_DIR_0  6 //45
#define N_DIR_1  38

const int n_node = 20;
const int n_order = 2;
const int n_dir[n_order] = { N_DIR_0, N_DIR_1 };
int*** dir_index = NULL;

#define complex std::complex<double>
#define EYE complex(0.0, 1.0)

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <cassert>
#include <exception>

void readDirIndex(int i_order, int** dir_index)
{
    using namespace boost::property_tree;
    try {
        ptree pt;
        read_json("../../cfg/index.json", pt);

        int i_dir = 0;
        char name[256];
        sprintf( name, "order.%d", i_order );
        BOOST_FOREACH(ptree::value_type &order, pt.get_child(name)) {
            assert(order.first.empty()); // array elements have no names
            int i_pulse = 0;
            BOOST_FOREACH(ptree::value_type& idx, order.second) {
                dir_index[i_dir][i_pulse] = idx.second.get_value<int>();
                printf("%d\n", idx.second.get_value<int>());
                i_pulse ++;
            }
            i_dir ++;
        }
    }
    catch (std::exception const& e) {
        fprintf( stderr, "%s\n", e.what() );
        exit(-1);
    }
}

void ini_dir_index() {
    dir_index = new int** [n_order];
    for (int i_order = 0; i_order < n_order; i_order ++) {
        dir_index[i_order] = new int* [n_dir[i_order]];
        for (int i_dir = 0; i_dir < n_dir[i_order]; i_dir ++)
            dir_index[i_order][i_dir] = new int [3];
    }
    readDirIndex( 1, dir_index[0] );
    if (n_order > 1)
        readDirIndex( 3, dir_index[1] );
}

// void ini_dir_index() {
//     dir_index = new int** [n_order];
//     for (int i_order = 0; i_order < n_order; i_order ++) {
//         dir_index[i_order] = new int* [n_dir[i_order]];
//         for (int i_dir = 0; i_dir < n_dir[i_order]; i_dir ++)
//             dir_index[i_order][i_dir] = new int [3]; // n_pulse
//     }
//     for (int i_pulse = 0; i_pulse < 3; i_pulse ++) {
//         for (int i_dir = 0; i_dir < n_dir[0]; i_dir ++)
//             dir_index[0][i_dir][i_pulse] = dir_index_0[i_dir][i_pulse];
//         if (n_order > 1)
//             for (int i_dir = 0; i_dir < n_dir[1]; i_dir ++)
//                 dir_index[1][i_dir][i_pulse] = dir_index_1[i_dir][i_pulse];
//     }
// }

void del_dir_index() {
    for (int i_order = 0; i_order < n_order; i_order ++) {
        for (int i_dir = 0; i_dir < n_dir[i_order]; i_dir ++)
            delete[] dir_index[i_order][i_dir];
        delete[] dir_index[i_order];
    }
    delete[] dir_index;
}

double spatial_phase_kr( int i_dir, int** index, parameters* ps ) {
    double phase = 0.0;
    for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            phase += index[i_dir][i_pulse] * ps->ef[i_pulse]->k0
                * ps->ef[i_pulse]->kuvL[i_dim] * ps->pos[i_dim];
    return phase;
}

struct st_par {
    complex val;
};

void loopAssign_1( complex*** pol_0,
                   void (*func)( complex&, st_par* ),
                   st_par* par, parameters* ps ) {
    for (long is = 0; is < ps->mpic->njob; is ++)
        for (long it = 0; it < ps->nt; it ++) {
            long index = (ps->mpic->idx0 + is) * (ps->nt) + it;
            for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
                for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                    func( pol_0[index][i_dpl][i_dim], par );
                }
        }
}

void loopAssign_2( complex*** pol_0, complex*** pol_1,
                   void (*func)( complex&, complex&, st_par* ),
                   st_par* par, parameters* ps ) {
    for (long is = 0; is < ps->mpic->njob; is ++)
        for (long it = 0; it < ps->nt; it ++) {
            long index = (ps->mpic->idx0 + is) * (ps->nt) + it;
            for (int i_dpl = 0; i_dpl < ps->pols->n_dpl; i_dpl ++)
                for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                    func( pol_0[index][i_dpl][i_dim], pol_1[index][i_dpl][i_dim], par );
                }
        }
}

inline void sum_up_for_ppar( complex& a, complex& b, st_par* par ) {
    b += a * par->val; }
inline void ptot_from_ppar_0( complex& a, complex& b, st_par* par ) {
    b += a * par->val; }
inline void reset_ptot_0( complex& a, st_par* par ) {
    a = 0.0; }
inline void substract_ptot0_from_ptot( complex& a, complex& b, st_par* par ) {
    b -= a; }
inline void normalize( complex& a, st_par* par ) {
    a /= par->val; }

void pullerits_order_0( complex**** ptot, complex***** ppar, parameters* ps ) {
    st_par par;
    // loop over all direction dir_0
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++) {
        double phase = spatial_phase_kr( i_dir, dir_index[0], ps );
        par.val = exp( - EYE * phase );
        loopAssign_2( ptot[0], ppar[0][i_dir], sum_up_for_ppar, &par, ps );
    }
}

void pullerits_order_1( complex**** ptot, complex***** ppar, parameters* ps ) {
    st_par par;
    // reset ptot[1] to zeros
    loopAssign_1( ptot[1], reset_ptot_0, NULL, ps );
    // sum of dir_0 with the obtained ppar_2d_0 CHK
    for (int i_dir = 0; i_dir < N_DIR_0; i_dir ++) {
        double phase = spatial_phase_kr( i_dir, dir_index[0], ps );
        par.val = exp( + EYE * phase );
        loopAssign_2( ppar[0][i_dir], ptot[1], ptot_from_ppar_0, &par, ps );
    }
    // substract ptot_1 = ptot - ptot_0 CHK
    loopAssign_2( ptot[1], ptot[0], substract_ptot0_from_ptot, NULL, ps );
    // loop over all direction dir_0
    for (int i_dir = 0; i_dir < N_DIR_1; i_dir ++) {
        double phase = spatial_phase_kr( i_dir, dir_index[1], ps );
        par.val = exp( -EYE * phase );
        // fill ppar_2d_1 CHK, loop over index and i_dim
        loopAssign_2( ptot[0], ppar[1][i_dir], sum_up_for_ppar, &par, ps );
    }
}

void ppar_extract_pullerits_process( int i_order, complex**** ptot,
                                     complex***** ppar, parameters* ps )
{
    if (i_order == 0) {
        pullerits_order_0( ptot, ppar, ps );
    } else if (i_order == 1) {
        pullerits_order_1( ptot, ppar, ps );
    }
}

void ppar_extract_pullerits_main_frame( int i_order, complex***** ppar, const char* dirBase )
{
    // prepare parameters
    parameters ps; ps.f_eom = NULL;
    para_ini( &ps, dirBase );
    // create temporary array ptot. ptot[0] is used for file data
    // ptot[1] is used for work array, if necessary
    complex ****ptot = new complex*** [2];
    for (int i = 0; i < 2; i ++)
        ptot[i] = prepare_pol_array( 2, &ps );
    // loop over all samples from files of each MPI node
    for (int rank = 0; rank < n_node; rank ++) {
        // prepare parameter for each node
        parameters ps1; ps1.f_eom = NULL;
        para_ini( &ps1, dirBase, n_node, rank );
        // open files for ptot and rl
        int file_idx[1] = { (int)ps1.mpic->rank };
        open_para_file( para_file::RL, dirBase, &ps1, 1, NULL, file_idx, "r" );
        open_para_file( para_file::PTOT_2D, dirBase, &ps1, 1, NULL, file_idx, "r" );
        // loop over ensemble, process data block from a single file
        for (int i_esmb = 0; i_esmb < ps1.esmb->n_esmb; i_esmb ++) {
            if (i_esmb % 20000 == 0)
                fprintf( stdout, "order %1d: rank %2d of %2d, esmb %d of %ld\n",
                         i_order, rank, n_node, i_esmb, ps1.esmb->n_esmb );
            io_rl_read( &ps1 );
            io_pol_read( para_file::PTOT_2D, ptot[0], &ps1 );
            ppar_extract_pullerits_process( i_order, ptot, ppar, &ps1 );
        }
        // close files for ptot and rl
        close_para_file( para_file::PTOT_2D, &ps1 );
        close_para_file( para_file::RL, &ps1 );
        // clean parameter for each node
        para_del( &ps1 );
    }
    // normalization
    st_par par; par.val = ps.esmb->n_esmb;
    for (int i_dir = 0; i_dir < n_dir[i_order]; i_dir ++)
        loopAssign_1( ppar[i_order][i_dir], normalize, &par, &ps );
    // clean temporary array ptot
    for (int i = 0; i < 2; i ++)
        clean_pol_array( 2, ptot[i], &ps );
    delete[] ptot;
    // clean parameters
    para_del( &ps );
}

void ppar_extract_pullerits( const char* dirBase ) {
    // prepare parameters
    parameters ps; ps.f_eom = NULL;
    para_ini( &ps, dirBase );
    // prepare arrays ppar: 2 x n_dir x (nt x ns) x n_dpl x n_dim
    complex***** ppar = new complex**** [n_order];
    for (int i_order = 0; i_order < n_order; i_order ++) {
        ppar[i_order] = new complex*** [n_dir[i_order]];
        for (int i_dir = 0; i_dir < n_dir[i_order]; i_dir ++)
            ppar[i_order][i_dir] = prepare_pol_array( 2, &ps );
    }
    // main process and output results ppar for different orders
    for (int i_order = 0; i_order < n_order; i_order ++) {
        ppar_extract_pullerits_main_frame( i_order, ppar, dirBase );
        io_pol_dir_write( para_file::PPAR_2D, ppar[i_order],
                          n_dir[i_order], NULL, &ps );
    }
    // clean arrays ppar
    for (int i_order = 0; i_order < n_order; i_order ++) {
        for (int i_dir = 0; i_dir < n_dir[i_order]; i_dir ++)
            clean_pol_array( 2, ppar[i_order][i_dir], &ps );
        delete[] ppar[i_order];
    }
    delete[] ppar;
    // clean parameters
    para_del( &ps );
}

void display_dir_index() {
    FILE* file = fopen( "res/index.dat", "w" );
    for (int k = 0; k < n_order; k ++) {
        fprintf( file, "Order of index %d:\n", k );
        for (int j = 0; j < n_dir[k]; j ++) {
            fprintf( file, "%2d: ", j );
            for (int i = 0; i < 3; i ++)
                fprintf( file, "%4d", dir_index[k][j][i] );
            fprintf( file, "\n" );
        }
    }
    fclose( file );
}

int main( int argc, char* argv[] ) {
    // char cfg_file[] = "../../cfg/parameters.cfg";
    const char dirBase[] = "../../";
    postproc_collect_mpi_grid( n_node, dirBase );
    ini_dir_index();
    display_dir_index();
    ppar_extract_pullerits( dirBase );
    del_dir_index();
    fprintf( stdout, "Hasta la vista!\n" );
}
