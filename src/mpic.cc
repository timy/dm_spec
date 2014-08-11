#include "mpic.h"
#include "para.h"
#include "debug.h"

long assign_jobs( long rank, long size, long total, long *start )
{   // calculation for load balance
    long n_task = total / size;
    if ( total % size == 0 )
        *start = n_task * rank;
    else {
        n_task ++;
        long n_less = n_task * size - total;
        long n_more = size - n_less;
        if (rank < n_more)
            *start = n_task * rank;
        else {
            *start = n_task * n_more + (n_task - 1) * (rank - n_more);
            n_task --;
        }
    }
    return n_task;
}

#include "file.h"
void para_mpic_ini( parameters *ps, long size, long rank )
{
    ps->mpic = new para_mpic;
    para_mpic_set( ps, size, rank );
    
    int file_idx[1] = { (int) ps->mpic->rank };
    open_para_file_write( para_file::LOG, NULL, ps, 1, file_idx );
}

void para_mpic_del( parameters *ps )
{
    close_para_file( para_file::LOG, ps );
    delete ps->mpic;
}

void para_mpic_set( parameters *ps, long size, long rank )
{
    ps->mpic->size = size;
    ps->mpic->rank = rank;
    
    long n_total;
    if (ps->mvar->ny > 1) {     // 2D: task partition for the 2nd var
        n_total = ps->mvar->ny;
        ps->mvar->dimGrid = 1;
    }
    else {                      // 1D: task partition for ensemble
        n_total = ps->esmb->n_esmb;
        ps->mvar->dimGrid = 0;
    }
    ps->mpic->njob = assign_jobs( ps->mpic->rank, ps->mpic->size,
                                  n_total, &(ps->mpic->idx0) );
}
