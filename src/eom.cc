// EOM (full form)
#include "para.h"
#include "eom.h"
#include "field.h"
#include "debug.h"
#include <gsl/gsl_errno.h>
#include <cstdlib>
#include <cstdio>

int equation_of_motion_5lv( double t, const double y[], double *dy,
                            void *peom )
{
    parameters *ps = ((para_eom*) peom)->ps;
    double Rr0n0 = y[0];
    double Rr1n1 = y[1];
    double Rr2n2 = y[2];
    double Rr3n3 = y[3];
    double Rr1n0 = y[4];
    double Ir1n0 = y[5];
    double Rr2n0 = y[6];
    double Ir2n0 = y[7];
    double Rr2n1 = y[8];
    double Ir2n1 = y[9];
    double Rr3n0 = y[10];
    double Ir3n0 = y[11];
    double Rr3n1 = y[12];
    double Ir3n1 = y[13];
    double Rr3n2 = y[14];
    double Ir3n2 = y[15];
    double w1n0 = ps->energy[1] - ps->energy[0];
    double w2n0 = ps->energy[2] - ps->energy[0];
    double w2n1 = ps->energy[2] - ps->energy[1];
    double w3n0 = ps->energy[3] - ps->energy[0];
    double w3n1 = ps->energy[3] - ps->energy[1];
    double w3n2 = ps->energy[3] - ps->energy[2];
    double ef[3];
    field_real_components( ef, t, ps );
    double V1n0 = 0.0;
    double V2n0 = 0.0;
    double V2n1 = 0.0;
    double V3n0 = 0.0;
    double V3n1 = 0.0;
    double V3n2 = 0.0;
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        // You need to specify the dipole momentum explicitly here
        V1n0 += - ps->dipole[0][i_dim] * ef[i_dim];
        V2n0 += - ps->dipole[1][i_dim] * ef[i_dim];
        V3n1 += - ps->dipole[2][i_dim] * ef[i_dim];
        V3n2 += - ps->dipole[3][i_dim] * ef[i_dim];
    }
    // Maybe you need to account for dissipation here...
    dy[0] = 2*(Ir1n0*V1n0 + Ir2n0*V2n0 + Ir3n0*V3n0);
    dy[1] = -2*(Ir1n0*V1n0 - Ir2n1*V2n1 - Ir3n1*V3n1);
    dy[2] = -2*(Ir2n0*V2n0 + Ir2n1*V2n1 - Ir3n2*V3n2);
    dy[3] = -2*(Ir3n0*V3n0 + Ir3n1*V3n1 + Ir3n2*V3n2);
    dy[4] = Ir2n1*V2n0 + Ir2n0*V2n1 + Ir3n1*V3n0 + Ir3n0*V3n1 + Ir1n0*w1n0;
    dy[5] = -(Rr0n0*V1n0) + Rr1n1*V1n0 + Rr2n1*V2n0 - Rr2n0*V2n1 + Rr3n1*V3n0 - Rr3n0*V3n1 - Rr1n0*w1n0;
    dy[6] = -(Ir2n1*V1n0) + Ir1n0*V2n1 + Ir3n2*V3n0 + Ir3n0*V3n2 + Ir2n0*w2n0;
    dy[7] = Rr2n1*V1n0 - Rr0n0*V2n0 + Rr2n2*V2n0 - Rr1n0*V2n1 + Rr3n2*V3n0 - Rr3n0*V3n2 - Rr2n0*w2n0;
    dy[8] = -(Ir2n0*V1n0) - Ir1n0*V2n0 + Ir3n2*V3n1 + Ir3n1*V3n2 + Ir2n1*w2n1;
    dy[9] = Rr2n0*V1n0 - Rr1n0*V2n0 - Rr1n1*V2n1 + Rr2n2*V2n1 + Rr3n2*V3n1 - Rr3n1*V3n2 - Rr2n1*w2n1;
    dy[10] = -(Ir3n1*V1n0) - Ir3n2*V2n0 + Ir1n0*V3n1 + Ir2n0*V3n2 + Ir3n0*w3n0;
    dy[11] = Rr3n1*V1n0 + Rr3n2*V2n0 - Rr0n0*V3n0 + Rr3n3*V3n0 - Rr1n0*V3n1 - Rr2n0*V3n2 - Rr3n0*w3n0;
    dy[12] = -(Ir3n0*V1n0) - Ir3n2*V2n1 - Ir1n0*V3n0 + Ir2n1*V3n2 + Ir3n1*w3n1;
    dy[13] = Rr3n0*V1n0 + Rr3n2*V2n1 - Rr1n0*V3n0 - Rr1n1*V3n1 + Rr3n3*V3n1 - Rr2n1*V3n2 - Rr3n1*w3n1;
    dy[14] = -(Ir3n0*V2n0) - Ir3n1*V2n1 - Ir2n0*V3n0 - Ir2n1*V3n1 + Ir3n2*w3n2;
    dy[15] = Rr3n0*V2n0 + Rr3n1*V2n1 - Rr2n0*V3n0 - Rr2n1*V3n1 - Rr2n2*V3n2 + Rr3n3*V3n2 - Rr3n2*w3n2;
    return GSL_SUCCESS;
}

int equation_of_motion_7lv( double t, const double y[], double *dy,
                            void *peom )
{
    parameters *ps = ((para_eom*) peom)->ps;

    double Rr0n0 = y[0];
    double Rr1n1 = y[1];
    double Rr2n2 = y[2];
    double Rr3n3 = y[3];
    double Rr4n4 = y[4];
    double Rr5n5 = y[5];
    double Rr6n6 = y[6];
    double Rr1n0 = y[7];
    double Ir1n0 = y[8];
    double Rr2n0 = y[9];
    double Ir2n0 = y[10];
    double Rr2n1 = y[11];
    double Ir2n1 = y[12];
    double Rr3n0 = y[13];
    double Ir3n0 = y[14];
    double Rr3n1 = y[15];
    double Ir3n1 = y[16];
    double Rr3n2 = y[17];
    double Ir3n2 = y[18];
    double Rr4n0 = y[19];
    double Ir4n0 = y[20];
    double Rr4n1 = y[21];
    double Ir4n1 = y[22];
    double Rr4n2 = y[23];
    double Ir4n2 = y[24];
    double Rr4n3 = y[25];
    double Ir4n3 = y[26];
    double Rr5n0 = y[27];
    double Ir5n0 = y[28];
    double Rr5n1 = y[29];
    double Ir5n1 = y[30];
    double Rr5n2 = y[31];
    double Ir5n2 = y[32];
    double Rr5n3 = y[33];
    double Ir5n3 = y[34];
    double Rr5n4 = y[35];
    double Ir5n4 = y[36];
    double Rr6n0 = y[37];
    double Ir6n0 = y[38];
    double Rr6n1 = y[39];
    double Ir6n1 = y[40];
    double Rr6n2 = y[41];
    double Ir6n2 = y[42];
    double Rr6n3 = y[43];
    double Ir6n3 = y[44];
    double Rr6n4 = y[45];
    double Ir6n4 = y[46];
    double Rr6n5 = y[47];
    double Ir6n5 = y[48];
    double w1n0 = ps->energy[1] - ps->energy[0];
    double w2n0 = ps->energy[2] - ps->energy[0];
    double w2n1 = ps->energy[2] - ps->energy[1];
    double w3n0 = ps->energy[3] - ps->energy[0];
    double w3n1 = ps->energy[3] - ps->energy[1];
    double w3n2 = ps->energy[3] - ps->energy[2];
    double w4n0 = ps->energy[4] - ps->energy[0];
    double w4n1 = ps->energy[4] - ps->energy[1];
    double w4n2 = ps->energy[4] - ps->energy[2];
    double w4n3 = ps->energy[4] - ps->energy[3];
    double w5n0 = ps->energy[5] - ps->energy[0];
    double w5n1 = ps->energy[5] - ps->energy[1];
    double w5n2 = ps->energy[5] - ps->energy[2];
    double w5n3 = ps->energy[5] - ps->energy[3];
    double w5n4 = ps->energy[5] - ps->energy[4];
    double w6n0 = ps->energy[6] - ps->energy[0];
    double w6n1 = ps->energy[6] - ps->energy[1];
    double w6n2 = ps->energy[6] - ps->energy[2];
    double w6n3 = ps->energy[6] - ps->energy[3];
    double w6n4 = ps->energy[6] - ps->energy[4];
    double w6n5 = ps->energy[6] - ps->energy[5];
    double ef[3];
    field_real_components( ef, t, ps );
    double V1n0 = 0.0;
    double V2n0 = 0.0;
    double V2n1 = 0.0;
    double V3n0 = 0.0;
    double V3n1 = 0.0;
    double V3n2 = 0.0;
    double V4n0 = 0.0;
    double V4n1 = 0.0;
    double V4n2 = 0.0;
    double V4n3 = 0.0;
    double V5n0 = 0.0;
    double V5n1 = 0.0;
    double V5n2 = 0.0;
    double V5n3 = 0.0;
    double V5n4 = 0.0;
    double V6n0 = 0.0;
    double V6n1 = 0.0;
    double V6n2 = 0.0;
    double V6n3 = 0.0;
    double V6n4 = 0.0;
    double V6n5 = 0.0;
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        // You need to specify the dipole momentum explicitly here
        V1n0 += - ps->dipole[0][i_dim] * ef[i_dim];
        V2n0 += - ps->dipole[1][i_dim] * ef[i_dim];
        V2n1 += - ps->dipole[2][i_dim] * ef[i_dim];
        V3n0 += - ps->dipole[3][i_dim] * ef[i_dim];
        V3n1 += - ps->dipole[4][i_dim] * ef[i_dim];
        V3n2 += - ps->dipole[5][i_dim] * ef[i_dim];
        V4n0 += - ps->dipole[6][i_dim] * ef[i_dim];
        V4n1 += - ps->dipole[7][i_dim] * ef[i_dim];
        V4n2 += - ps->dipole[8][i_dim] * ef[i_dim];
        V4n3 += - ps->dipole[9][i_dim] * ef[i_dim];
        V5n0 += - ps->dipole[10][i_dim] * ef[i_dim];
        V5n1 += - ps->dipole[11][i_dim] * ef[i_dim];
        V5n2 += - ps->dipole[12][i_dim] * ef[i_dim];
        V5n3 += - ps->dipole[13][i_dim] * ef[i_dim];
        V5n4 += - ps->dipole[14][i_dim] * ef[i_dim];
        V6n0 += - ps->dipole[15][i_dim] * ef[i_dim];
        V6n1 += - ps->dipole[16][i_dim] * ef[i_dim];
        V6n2 += - ps->dipole[17][i_dim] * ef[i_dim];
        V6n3 += - ps->dipole[18][i_dim] * ef[i_dim];
        V6n4 += - ps->dipole[19][i_dim] * ef[i_dim];
        V6n5 += - ps->dipole[20][i_dim] * ef[i_dim];
    }
    // Maybe you need to account for dissipation here...
    dy[0] = 2*(Ir1n0*V1n0 + Ir2n0*V2n0 + Ir3n0*V3n0 + Ir4n0*V4n0 + Ir5n0*V5n0 + Ir6n0*V6n0);
    dy[1] = -2*(Ir1n0*V1n0 - Ir2n1*V2n1 - Ir3n1*V3n1 - Ir4n1*V4n1 - Ir5n1*V5n1 - Ir6n1*V6n1);
    dy[2] = -2*(Ir2n0*V2n0 + Ir2n1*V2n1 - Ir3n2*V3n2 - Ir4n2*V4n2 - Ir5n2*V5n2 - Ir6n2*V6n2);
    dy[3] = -2*(Ir3n0*V3n0 + Ir3n1*V3n1 + Ir3n2*V3n2 - Ir4n3*V4n3 - Ir5n3*V5n3 - Ir6n3*V6n3);
    dy[4] = -2*(Ir4n0*V4n0 + Ir4n1*V4n1 + Ir4n2*V4n2 + Ir4n3*V4n3 - Ir5n4*V5n4 - Ir6n4*V6n4);
    dy[5] = -2*(Ir5n0*V5n0 + Ir5n1*V5n1 + Ir5n2*V5n2 + Ir5n3*V5n3 + Ir5n4*V5n4 - Ir6n5*V6n5);
    dy[6] = -2*(Ir6n0*V6n0 + Ir6n1*V6n1 + Ir6n2*V6n2 + Ir6n3*V6n3 + Ir6n4*V6n4 + Ir6n5*V6n5);
    dy[7] = Ir2n1*V2n0 + Ir2n0*V2n1 + Ir3n1*V3n0 + Ir3n0*V3n1 + Ir4n1*V4n0 + Ir4n0*V4n1 + Ir5n1*V5n0 + Ir5n0*V5n1 + Ir6n1*V6n0 + Ir6n0*V6n1 + Ir1n0*w1n0;
    dy[8] = -(Rr0n0*V1n0) + Rr1n1*V1n0 + Rr2n1*V2n0 - Rr2n0*V2n1 + Rr3n1*V3n0 - Rr3n0*V3n1 + Rr4n1*V4n0 - Rr4n0*V4n1 + Rr5n1*V5n0 - Rr5n0*V5n1 + Rr6n1*V6n0 - Rr6n0*V6n1 - Rr1n0*w1n0;
    dy[9] = -(Ir2n1*V1n0) + Ir1n0*V2n1 + Ir3n2*V3n0 + Ir3n0*V3n2 + Ir4n2*V4n0 + Ir4n0*V4n2 + Ir5n2*V5n0 + Ir5n0*V5n2 + Ir6n2*V6n0 + Ir6n0*V6n2 + Ir2n0*w2n0;
    dy[10] = Rr2n1*V1n0 - Rr0n0*V2n0 + Rr2n2*V2n0 - Rr1n0*V2n1 + Rr3n2*V3n0 - Rr3n0*V3n2 + Rr4n2*V4n0 - Rr4n0*V4n2 + Rr5n2*V5n0 - Rr5n0*V5n2 + Rr6n2*V6n0 - Rr6n0*V6n2 - Rr2n0*w2n0;
    dy[11] = -(Ir2n0*V1n0) - Ir1n0*V2n0 + Ir3n2*V3n1 + Ir3n1*V3n2 + Ir4n2*V4n1 + Ir4n1*V4n2 + Ir5n2*V5n1 + Ir5n1*V5n2 + Ir6n2*V6n1 + Ir6n1*V6n2 + Ir2n1*w2n1;
    dy[12] = Rr2n0*V1n0 - Rr1n0*V2n0 - Rr1n1*V2n1 + Rr2n2*V2n1 + Rr3n2*V3n1 - Rr3n1*V3n2 + Rr4n2*V4n1 - Rr4n1*V4n2 + Rr5n2*V5n1 - Rr5n1*V5n2 + Rr6n2*V6n1 - Rr6n1*V6n2 - Rr2n1*w2n1;
    dy[13] = -(Ir3n1*V1n0) - Ir3n2*V2n0 + Ir1n0*V3n1 + Ir2n0*V3n2 + Ir4n3*V4n0 + Ir4n0*V4n3 + Ir5n3*V5n0 + Ir5n0*V5n3 + Ir6n3*V6n0 + Ir6n0*V6n3 + Ir3n0*w3n0;
    dy[14] = Rr3n1*V1n0 + Rr3n2*V2n0 - Rr0n0*V3n0 + Rr3n3*V3n0 - Rr1n0*V3n1 - Rr2n0*V3n2 + Rr4n3*V4n0 - Rr4n0*V4n3 + Rr5n3*V5n0 - Rr5n0*V5n3 + Rr6n3*V6n0 - Rr6n0*V6n3 - Rr3n0*w3n0;
    dy[15] = -(Ir3n0*V1n0) - Ir3n2*V2n1 - Ir1n0*V3n0 + Ir2n1*V3n2 + Ir4n3*V4n1 + Ir4n1*V4n3 + Ir5n3*V5n1 + Ir5n1*V5n3 + Ir6n3*V6n1 + Ir6n1*V6n3 + Ir3n1*w3n1;
    dy[16] = Rr3n0*V1n0 + Rr3n2*V2n1 - Rr1n0*V3n0 - Rr1n1*V3n1 + Rr3n3*V3n1 - Rr2n1*V3n2 + Rr4n3*V4n1 - Rr4n1*V4n3 + Rr5n3*V5n1 - Rr5n1*V5n3 + Rr6n3*V6n1 - Rr6n1*V6n3 - Rr3n1*w3n1;
    dy[17] = -(Ir3n0*V2n0) - Ir3n1*V2n1 - Ir2n0*V3n0 - Ir2n1*V3n1 + Ir4n3*V4n2 + Ir4n2*V4n3 + Ir5n3*V5n2 + Ir5n2*V5n3 + Ir6n3*V6n2 + Ir6n2*V6n3 + Ir3n2*w3n2;
    dy[18] = Rr3n0*V2n0 + Rr3n1*V2n1 - Rr2n0*V3n0 - Rr2n1*V3n1 - Rr2n2*V3n2 + Rr3n3*V3n2 + Rr4n3*V4n2 - Rr4n2*V4n3 + Rr5n3*V5n2 - Rr5n2*V5n3 + Rr6n3*V6n2 - Rr6n2*V6n3 - Rr3n2*w3n2;
    dy[19] = -(Ir4n1*V1n0) - Ir4n2*V2n0 - Ir4n3*V3n0 + Ir1n0*V4n1 + Ir2n0*V4n2 + Ir3n0*V4n3 + Ir5n4*V5n0 + Ir5n0*V5n4 + Ir6n4*V6n0 + Ir6n0*V6n4 + Ir4n0*w4n0;
    dy[20] = Rr4n1*V1n0 + Rr4n2*V2n0 + Rr4n3*V3n0 - Rr0n0*V4n0 + Rr4n4*V4n0 - Rr1n0*V4n1 - Rr2n0*V4n2 - Rr3n0*V4n3 + Rr5n4*V5n0 - Rr5n0*V5n4 + Rr6n4*V6n0 - Rr6n0*V6n4 - Rr4n0*w4n0;
    dy[21] = -(Ir4n0*V1n0) - Ir4n2*V2n1 - Ir4n3*V3n1 - Ir1n0*V4n0 + Ir2n1*V4n2 + Ir3n1*V4n3 + Ir5n4*V5n1 + Ir5n1*V5n4 + Ir6n4*V6n1 + Ir6n1*V6n4 + Ir4n1*w4n1;
    dy[22] = Rr4n0*V1n0 + Rr4n2*V2n1 + Rr4n3*V3n1 - Rr1n0*V4n0 - Rr1n1*V4n1 + Rr4n4*V4n1 - Rr2n1*V4n2 - Rr3n1*V4n3 + Rr5n4*V5n1 - Rr5n1*V5n4 + Rr6n4*V6n1 - Rr6n1*V6n4 - Rr4n1*w4n1;
    dy[23] = -(Ir4n0*V2n0) - Ir4n1*V2n1 - Ir4n3*V3n2 - Ir2n0*V4n0 - Ir2n1*V4n1 + Ir3n2*V4n3 + Ir5n4*V5n2 + Ir5n2*V5n4 + Ir6n4*V6n2 + Ir6n2*V6n4 + Ir4n2*w4n2;
    dy[24] = Rr4n0*V2n0 + Rr4n1*V2n1 + Rr4n3*V3n2 - Rr2n0*V4n0 - Rr2n1*V4n1 - Rr2n2*V4n2 + Rr4n4*V4n2 - Rr3n2*V4n3 + Rr5n4*V5n2 - Rr5n2*V5n4 + Rr6n4*V6n2 - Rr6n2*V6n4 - Rr4n2*w4n2;
    dy[25] = -(Ir4n0*V3n0) - Ir4n1*V3n1 - Ir4n2*V3n2 - Ir3n0*V4n0 - Ir3n1*V4n1 - Ir3n2*V4n2 + Ir5n4*V5n3 + Ir5n3*V5n4 + Ir6n4*V6n3 + Ir6n3*V6n4 + Ir4n3*w4n3;
    dy[26] = Rr4n0*V3n0 + Rr4n1*V3n1 + Rr4n2*V3n2 - Rr3n0*V4n0 - Rr3n1*V4n1 - Rr3n2*V4n2 - Rr3n3*V4n3 + Rr4n4*V4n3 + Rr5n4*V5n3 - Rr5n3*V5n4 + Rr6n4*V6n3 - Rr6n3*V6n4 - Rr4n3*w4n3;
    dy[27] = -(Ir5n1*V1n0) - Ir5n2*V2n0 - Ir5n3*V3n0 - Ir5n4*V4n0 + Ir1n0*V5n1 + Ir2n0*V5n2 + Ir3n0*V5n3 + Ir4n0*V5n4 + Ir6n5*V6n0 + Ir6n0*V6n5 + Ir5n0*w5n0;
    dy[28] = Rr5n1*V1n0 + Rr5n2*V2n0 + Rr5n3*V3n0 + Rr5n4*V4n0 - Rr0n0*V5n0 + Rr5n5*V5n0 - Rr1n0*V5n1 - Rr2n0*V5n2 - Rr3n0*V5n3 - Rr4n0*V5n4 + Rr6n5*V6n0 - Rr6n0*V6n5 - Rr5n0*w5n0;
    dy[29] = -(Ir5n0*V1n0) - Ir5n2*V2n1 - Ir5n3*V3n1 - Ir5n4*V4n1 - Ir1n0*V5n0 + Ir2n1*V5n2 + Ir3n1*V5n3 + Ir4n1*V5n4 + Ir6n5*V6n1 + Ir6n1*V6n5 + Ir5n1*w5n1;
    dy[30] = Rr5n0*V1n0 + Rr5n2*V2n1 + Rr5n3*V3n1 + Rr5n4*V4n1 - Rr1n0*V5n0 - Rr1n1*V5n1 + Rr5n5*V5n1 - Rr2n1*V5n2 - Rr3n1*V5n3 - Rr4n1*V5n4 + Rr6n5*V6n1 - Rr6n1*V6n5 - Rr5n1*w5n1;
    dy[31] = -(Ir5n0*V2n0) - Ir5n1*V2n1 - Ir5n3*V3n2 - Ir5n4*V4n2 - Ir2n0*V5n0 - Ir2n1*V5n1 + Ir3n2*V5n3 + Ir4n2*V5n4 + Ir6n5*V6n2 + Ir6n2*V6n5 + Ir5n2*w5n2;
    dy[32] = Rr5n0*V2n0 + Rr5n1*V2n1 + Rr5n3*V3n2 + Rr5n4*V4n2 - Rr2n0*V5n0 - Rr2n1*V5n1 - Rr2n2*V5n2 + Rr5n5*V5n2 - Rr3n2*V5n3 - Rr4n2*V5n4 + Rr6n5*V6n2 - Rr6n2*V6n5 - Rr5n2*w5n2;
    dy[33] = -(Ir5n0*V3n0) - Ir5n1*V3n1 - Ir5n2*V3n2 - Ir5n4*V4n3 - Ir3n0*V5n0 - Ir3n1*V5n1 - Ir3n2*V5n2 + Ir4n3*V5n4 + Ir6n5*V6n3 + Ir6n3*V6n5 + Ir5n3*w5n3;
    dy[34] = Rr5n0*V3n0 + Rr5n1*V3n1 + Rr5n2*V3n2 + Rr5n4*V4n3 - Rr3n0*V5n0 - Rr3n1*V5n1 - Rr3n2*V5n2 - Rr3n3*V5n3 + Rr5n5*V5n3 - Rr4n3*V5n4 + Rr6n5*V6n3 - Rr6n3*V6n5 - Rr5n3*w5n3;
    dy[35] = -(Ir5n0*V4n0) - Ir5n1*V4n1 - Ir5n2*V4n2 - Ir5n3*V4n3 - Ir4n0*V5n0 - Ir4n1*V5n1 - Ir4n2*V5n2 - Ir4n3*V5n3 + Ir6n5*V6n4 + Ir6n4*V6n5 + Ir5n4*w5n4;
    dy[36] = Rr5n0*V4n0 + Rr5n1*V4n1 + Rr5n2*V4n2 + Rr5n3*V4n3 - Rr4n0*V5n0 - Rr4n1*V5n1 - Rr4n2*V5n2 - Rr4n3*V5n3 - Rr4n4*V5n4 + Rr5n5*V5n4 + Rr6n5*V6n4 - Rr6n4*V6n5 - Rr5n4*w5n4;
    dy[37] = -(Ir6n1*V1n0) - Ir6n2*V2n0 - Ir6n3*V3n0 - Ir6n4*V4n0 - Ir6n5*V5n0 + Ir1n0*V6n1 + Ir2n0*V6n2 + Ir3n0*V6n3 + Ir4n0*V6n4 + Ir5n0*V6n5 + Ir6n0*w6n0;
    dy[38] = Rr6n1*V1n0 + Rr6n2*V2n0 + Rr6n3*V3n0 + Rr6n4*V4n0 + Rr6n5*V5n0 - Rr0n0*V6n0 + Rr6n6*V6n0 - Rr1n0*V6n1 - Rr2n0*V6n2 - Rr3n0*V6n3 - Rr4n0*V6n4 - Rr5n0*V6n5 - Rr6n0*w6n0;
    dy[39] = -(Ir6n0*V1n0) - Ir6n2*V2n1 - Ir6n3*V3n1 - Ir6n4*V4n1 - Ir6n5*V5n1 - Ir1n0*V6n0 + Ir2n1*V6n2 + Ir3n1*V6n3 + Ir4n1*V6n4 + Ir5n1*V6n5 + Ir6n1*w6n1;
    dy[40] = Rr6n0*V1n0 + Rr6n2*V2n1 + Rr6n3*V3n1 + Rr6n4*V4n1 + Rr6n5*V5n1 - Rr1n0*V6n0 - Rr1n1*V6n1 + Rr6n6*V6n1 - Rr2n1*V6n2 - Rr3n1*V6n3 - Rr4n1*V6n4 - Rr5n1*V6n5 - Rr6n1*w6n1;
    dy[41] = -(Ir6n0*V2n0) - Ir6n1*V2n1 - Ir6n3*V3n2 - Ir6n4*V4n2 - Ir6n5*V5n2 - Ir2n0*V6n0 - Ir2n1*V6n1 + Ir3n2*V6n3 + Ir4n2*V6n4 + Ir5n2*V6n5 + Ir6n2*w6n2;
    dy[42] = Rr6n0*V2n0 + Rr6n1*V2n1 + Rr6n3*V3n2 + Rr6n4*V4n2 + Rr6n5*V5n2 - Rr2n0*V6n0 - Rr2n1*V6n1 - Rr2n2*V6n2 + Rr6n6*V6n2 - Rr3n2*V6n3 - Rr4n2*V6n4 - Rr5n2*V6n5 - Rr6n2*w6n2;
    dy[43] = -(Ir6n0*V3n0) - Ir6n1*V3n1 - Ir6n2*V3n2 - Ir6n4*V4n3 - Ir6n5*V5n3 - Ir3n0*V6n0 - Ir3n1*V6n1 - Ir3n2*V6n2 + Ir4n3*V6n4 + Ir5n3*V6n5 + Ir6n3*w6n3;
    dy[44] = Rr6n0*V3n0 + Rr6n1*V3n1 + Rr6n2*V3n2 + Rr6n4*V4n3 + Rr6n5*V5n3 - Rr3n0*V6n0 - Rr3n1*V6n1 - Rr3n2*V6n2 - Rr3n3*V6n3 + Rr6n6*V6n3 - Rr4n3*V6n4 - Rr5n3*V6n5 - Rr6n3*w6n3;
    dy[45] = -(Ir6n0*V4n0) - Ir6n1*V4n1 - Ir6n2*V4n2 - Ir6n3*V4n3 - Ir6n5*V5n4 - Ir4n0*V6n0 - Ir4n1*V6n1 - Ir4n2*V6n2 - Ir4n3*V6n3 + Ir5n4*V6n5 + Ir6n4*w6n4;
    dy[46] = Rr6n0*V4n0 + Rr6n1*V4n1 + Rr6n2*V4n2 + Rr6n3*V4n3 + Rr6n5*V5n4 - Rr4n0*V6n0 - Rr4n1*V6n1 - Rr4n2*V6n2 - Rr4n3*V6n3 - Rr4n4*V6n4 + Rr6n6*V6n4 - Rr5n4*V6n5 - Rr6n4*w6n4;
    dy[47] = -(Ir6n0*V5n0) - Ir6n1*V5n1 - Ir6n2*V5n2 - Ir6n3*V5n3 - Ir6n4*V5n4 - Ir5n0*V6n0 - Ir5n1*V6n1 - Ir5n2*V6n2 - Ir5n3*V6n3 - Ir5n4*V6n4 + Ir6n5*w6n5;
    dy[48] = Rr6n0*V5n0 + Rr6n1*V5n1 + Rr6n2*V5n2 + Rr6n3*V5n3 + Rr6n4*V5n4 - Rr5n0*V6n0 - Rr5n1*V6n1 - Rr5n2*V6n2 - Rr5n3*V6n3 - Rr5n4*V6n4 - Rr5n5*V6n5 + Rr6n6*V6n5 - Rr6n5*w6n5;
    return GSL_SUCCESS;
}

int equation_of_motion_full( double t, const double y[], double *dy,
                             void *peom )
{ 
    parameters *ps = ((para_eom*) peom)->ps;

    double Rr00 = y[0];
    double Rr11 = y[1];
    double Rr22 = y[2];
    double Rr33 = y[3];
    double Rr10 = y[4];
    double Ir10 = y[5];
    double Rr20 = y[6];
    double Ir20 = y[7];
    double Rr31 = y[8];
    double Ir31 = y[9];
    double Rr32 = y[10];
    double Ir32 = y[11];
    double Rr12 = y[12];
    double Ir12 = y[13];
    double Rr30 = y[14];
    double Ir30 = y[15];

    double w10, w20, w12, w31, w32, w30;
    w10 = (ps->energy[1] - ps->energy[0]);
    w20 = (ps->energy[2] - ps->energy[0]);
    w12 = (ps->energy[1] - ps->energy[2]);
    w31 = (ps->energy[3] - ps->energy[1]);
    w32 = (ps->energy[3] - ps->energy[2]);
    w30 = (ps->energy[3] - ps->energy[0]);

    double mu_10[3], mu_20[3], mu_31[3], mu_32[3];
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        mu_10[i_dim] = ps->dipole[0][i_dim];
        mu_20[i_dim] = ps->dipole[1][i_dim];
        mu_31[i_dim] = ps->dipole[2][i_dim];
        mu_32[i_dim] = ps->dipole[3][i_dim];
    }

    double ef[3]; // 3D components of total field
    field_real_components( ef, t, ps );
    double V10 = 0.0, V20 = 0.0, V31 = 0.0, V32 = 0.0;
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
        V10 += - mu_10[i_dim] * ef[i_dim];
        V20 += - mu_20[i_dim] * ef[i_dim];
        V31 += - mu_31[i_dim] * ef[i_dim];
        V32 += - mu_32[i_dim] * ef[i_dim];
    }

    // dissipation
    double g12 = ps->bath->g12;
    double g21 = ps->bath->g21;
    double G11 = ps->bath->G11;
    double G22 = ps->bath->G22;
    double G12 = ps->bath->G12;
    double G10 = ps->bath->G10;
    double G20 = ps->bath->G20;
    double G31 = ps->bath->G31;
    double G32 = ps->bath->G32;
    double G30 = ps->bath->G30;

    // Re r00
    dy[0] = 2.0 * (Ir10 * V10 + Ir20 * V20);

    // Re r11
    dy[1] = 2.0 * (Ir31 * V31 - Ir10 * V10)
        - G11*Rr11 + g12*Rr22;
    
    // Re r22
    dy[2] = 2.0 * (Ir32 * V32 - Ir20 * V20)
        - G22*Rr22 + g21*Rr11;
    
    // Re r33
    dy[3] = -2.0 * (Ir31 * V31 + Ir32 * V32);

    // Re r10
    dy[4] = -Ir12*V20 + Ir30*V31 + Ir10*w10
        - G10*Rr10;

    // Im r10
    dy[5] = (Rr11-Rr00)*V10 + Rr12*V20 - Rr30*V31 - Rr10*w10
        - G10*Ir10;

    // Re r20
    dy[6] = Ir12*V10 + Ir30*V32 + Ir20*w20
        - G20*Rr20;
            
    // Im r20
    dy[7] = (Rr22-Rr00)*V20 + Rr12*V10 - Rr30*V32 - Rr20*w20
        - G20*Ir20;

    // Re r31
    dy[8] = -Ir30*V10 - Ir12*V32 + Ir31*w31
        - G31*Rr31;

    // Im r31
    dy[9] = (Rr33-Rr11)*V31 + Rr30*V10 - Rr12*V32 - Rr31*w31
        - G31*Ir31;

    // Re r32
    dy[10] = -Ir30*V20 + Ir12*V31 + Ir32*w32
        - G32*Rr32;

    // Im r32
    dy[11] = (Rr33-Rr22)*V32 + Rr30*V20 - Rr12*V31 - Rr32*w32
        - G32*Ir32;

    // Re r12
    dy[12] = -Ir20*V10 - Ir10*V20 + Ir32*V31 + Ir31*V32 + Ir12*w12
        - G12*Rr12;

    // Im r12
    dy[13] = -Rr20*V10 + Rr10*V20 - Rr32*V31 + Rr31*V32 - Rr12*w12
        - G12*Ir12;

    // Re r30
    dy[14] = -Ir31*V10 - Ir32*V20 + Ir10*V31 + Ir20*V32 + Ir30*w30
        - G30*Rr30;

    // Im r30
    dy[15] =  Rr31*V10 + Rr32*V20 - Rr10*V31 - Rr20*V32 - Rr30*w30
        - G30*Ir30;

    return GSL_SUCCESS;
}

#include "complex"
#define complex std::complex<double>

void eom_output_dm( int it, para_eom *peom );
void eom_output_ef( int it, para_eom* peom );

void eom_init_cond( double *y, para_eom *peom )
{
    // Don't forget to change the value of population for levels.
    y[0] = 1.0;
    for (int ie = 1; ie < peom->ps->n_eom; ie ++)
        y[ie] = 0.0;

    for (int ie = 0; ie < peom->ps->n_eom; ie ++)
        peom->rho[0][ie] = y[ie];

#ifndef USE_MPI
    eom_output_dm( 0, peom );
    eom_output_ef( 0, peom );
#endif
}

void eom_post_proc( int it, double *y, para_eom *peom )    
{
    for (int ie = 0; ie < peom->ps->n_eom; ie ++)
        peom->rho[it][ie] = y[ie];

#ifndef USE_MPI
    eom_output_dm( it, peom );
    eom_output_ef( it, peom );
#endif

}

void eom_output_dm( int it, para_eom* peom )
{
    FILE* f_dm = peom->ps->file->one[para_file::DM]->fptr;
    for (int ie = 0; ie < peom->ps->n_eom; ie ++)
        fprintf( f_dm, "%le ", peom->rho[it][ie] );
    fprintf( f_dm, "\n" );
}

void eom_output_ef( int it, para_eom* peom )
{
    FILE* f_ef = peom->ps->file->one[para_file::EF]->fptr;
    double* ef = new double [peom->ps->n_pulse];
    field_real_components( ef, peom->ps->time[it], peom->ps );
    for (int i_dim = 0; i_dim < peom->ps->n_dim; i_dim ++)
        fprintf( f_ef, "%le ", ef[i_dim] /*/ peom->ps->ef[0]->E0*/ );
    fprintf( f_ef, "\n" );
    delete[] ef;    
}
