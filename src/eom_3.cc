#include "eom_header.cc"

int equation_of_motion_run_3( double t, const double y[], double* dy, void* peom )
{
#include "eom_pre.cc"

    dy[0] = 2*Ir[1][0]*V[1][0] + 2*Ir[2][0]*V[2][0] + Rr[0][0]*R[0][0][0][0] + Rr[1][1]*R[0][0][1][1] + Rr[2][2]*R[0][0][2][2];
    dy[1] = -2*Ir[1][0]*V[1][0] + 2*Ir[2][1]*V[2][1] + Rr[0][0]*R[1][1][0][0] + Rr[1][1]*R[1][1][1][1] + Rr[2][2]*R[1][1][2][2];
    dy[2] = -2*Ir[2][0]*V[2][0] - 2*Ir[2][1]*V[2][1] + Rr[0][0]*R[2][2][0][0] + Rr[1][1]*R[2][2][1][1] + Rr[2][2]*R[2][2][2][2];
    dy[3] = Ir[2][1]*V[2][0] + Ir[2][0]*V[2][1] + Ir[1][0]*w[1][0] + Rr[1][0]*R[1][0][1][0];
    dy[4] = -(Rr[0][0]*V[1][0]) + Rr[1][1]*V[1][0] + Rr[2][1]*V[2][0] - Rr[2][0]*V[2][1] - Rr[1][0]*w[1][0] + Ir[1][0]*R[1][0][1][0];
    dy[5] = -(Ir[2][1]*V[1][0]) + Ir[1][0]*V[2][1] + Ir[2][0]*w[2][0] + Rr[2][0]*R[2][0][2][0];
    dy[6] = Rr[2][1]*V[1][0] - Rr[0][0]*V[2][0] + Rr[2][2]*V[2][0] - Rr[1][0]*V[2][1] - Rr[2][0]*w[2][0] + Ir[2][0]*R[2][0][2][0];
    dy[7] = -(Ir[2][0]*V[1][0]) - Ir[1][0]*V[2][0] + Ir[2][1]*w[2][1] + Rr[2][1]*R[2][1][2][1];
    dy[8] = Rr[2][0]*V[1][0] - Rr[1][0]*V[2][0] - Rr[1][1]*V[2][1] + Rr[2][2]*V[2][1] - Rr[2][1]*w[2][1] + Ir[2][1]*R[2][1][2][1];

    return GSL_SUCCESS;
}
