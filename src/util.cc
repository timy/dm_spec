#include "util.h"
#include <cstdio>
#include <ctime>

static clock_t time_start;

void tic()
{
    time_start = clock();
}

void tac( FILE *file )
{
    double duration = double (clock() - time_start) / CLOCKS_PER_SEC;
    fprintf( stdout, "Time elapses: %f s\n", duration );
}
