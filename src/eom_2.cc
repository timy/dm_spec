#include "eom_header.cc"

int equation_of_motion_run_2( double t, const double y[], double* dy, void* peom )
{
#include "eom_pre.cc"

    dy[0] = -R[0][0] + 2*Ir[1][0]*V[1][0];
    dy[1] = -R[1][1] - 2*Ir[1][0]*V[1][0];
    dy[2] = -R[1][0] + Ir[1][0]*w[1][0];
    dy[3] = -(Rr[0][0]*V[1][0]) + Rr[1][1]*V[1][0] - Rr[1][0]*w[1][0];

    return GSL_SUCCESS;
}
