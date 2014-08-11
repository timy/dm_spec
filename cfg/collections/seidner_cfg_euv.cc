#include "seidner_cfg.h"
#include "para.h"

// number of directions for phase matching
#define N_PHASE 61 // 25 37 61

void seidner_size( parameters *ps )
{
    ps->seid->n_phase = N_PHASE;
}

void seidner_indice( parameters *ps )
{
    int index[N_PHASE][N_PULSE] = {
        // 1st
        { 1,  0 }, { 0,  1 },  //  0,  1
        // 2nd
        { 2,  0 }, { 0,  2 },  //  2,  3
        { 1,  1 }, { 1, -1 },  //  4,  5
        // 3rd
        { 3,  0 }, { 0,  3 },  //  6,  7
        { 1,  2 }, { 2,  1 },  //  8,  9
        { 1, -2 }, {-2,  1 },  //  10, 11
        // 4th
        { 4,  0 }, { 0,  4 },  //  12, 13
        { 1,  3 }, { 3,  1 },  //  14, 15
        { 1, -3 }, {-3,  1 },  //  16, 17
        { 2,  2 }, { 2, -2 },  //  18, 19
        // 5th
        { 5,  0 }, { 0,  5 },  //  20, 21
        { 1,  4 }, { 4,  1 },  //  22, 23
        { 1, -4 }, {-4,  1 },  //  24, 25
        { 2,  3 }, { 3,  2 },  //  26, 27
        { 2, -3 }, {-3,  2 },  //  28, 29

        // 1st
        {-1,  0 }, { 0, -1 },  //  30, 31
        // 2nd
        {-2,  0 }, { 0, -2 },  //  32, 33
        {-1, -1 }, {-1,  1 },  //  34, 35
        // 3rd
        {-3,  0 }, { 0, -3 },  //  36, 37
        {-1, -2 }, {-2, -1 },  //  38, 39
        {-1,  2 }, { 2, -1 },  //  40, 41
        // 4th
        {-4,  0 }, { 0, -4 },  //  42, 43
        {-1, -3 }, {-3, -1 },  //  44, 45
        {-1,  3 }, { 3, -1 },  //  46, 47
        {-2, -2 }, {-2,  2 },  //  48, 49
        // 5th
        {-5,  0 }, { 0, -5 },  //  50, 51
        {-1, -4 }, {-4, -1 },  //  52, 53
        {-1,  4 }, { 4, -1 },  //  54, 55
        {-2, -3 }, {-3, -2 },  //  56, 57
        {-2,  3 }, { 3, -2 },  //  58, 59

        { 0,  0 }
    };

    for (int i = 0; i < N_PHASE; i ++)
        for (int j = 0; j < N_PULSE; j ++)
            ps->seid->index[i][j] = index[i][j];
}

#include <cstdlib>
void seidner_position( parameters *ps )
{
    for (int i_phi = 0; i_phi < N_PHASE; i_phi ++)
        for (int i_dim = 0; i_dim < N_DIM; i_dim ++)
            // ps->seid->rM[i_phi][i_dim] =
            ps->seid->rL[i_phi][i_dim] = 
                (rand() * 1.0 / RAND_MAX - 0.5) * 2e-6;
}
