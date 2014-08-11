#ifndef MPI_CFG_H_
#define MPI_CFG_H_

struct mpi_configure
{
    long n_task_per_cpu;
};

void read_mpi_cfg( mpi_configure *cfg );

long assign_jobs( long rank, long size, long total, long *start );

#endif
