#include "rotation.h"
#include <cmath>

void rotation_matrix( double phi, double theta, double psi,
                      double **mxRotB, double **mxRotF )
{
  // mxRotB and mxRotF are 3x3 matrice
  double c_phi   = cos( phi );
  double c_theta = cos( theta );
  double c_psi   = cos( psi );
  double s_phi   = sin( phi );
  double s_theta = sin( theta );
  double s_psi   = sin( psi );
  // backward rotation
  mxRotB[0][0] = - c_theta * s_phi * s_psi + c_phi * c_psi;
  mxRotB[0][1] = + c_theta * c_phi * s_psi + s_phi * c_psi;
  mxRotB[0][2] = + s_theta * s_psi;
  mxRotB[1][0] = - c_theta * s_phi * c_psi - c_phi * s_psi;
  mxRotB[1][1] = + c_theta * c_phi * c_psi - s_phi * s_psi;
  mxRotB[1][2] = + s_theta * c_psi;
  mxRotB[2][0] = + s_theta * s_phi;
  mxRotB[2][1] = - s_theta * c_phi;
  mxRotB[2][2] = + c_theta;
  // forward rotation - transpose of backward transformation
  mxRotF[0][0] = mxRotB[0][0];
  mxRotF[0][1] = mxRotB[1][0];
  mxRotF[0][2] = mxRotB[2][0];
  mxRotF[1][0] = mxRotB[0][1];
  mxRotF[1][1] = mxRotB[1][1];
  mxRotF[1][2] = mxRotB[2][1];
  mxRotF[2][0] = mxRotB[0][2];
  mxRotF[2][1] = mxRotB[1][2];
  mxRotF[2][2] = mxRotB[2][2];
}

void rotate_transform( const double *v_a, double *v_b, double **mxRot )
{
  // Usage: call rotation_matrix first.
  // mxRot = mxRotF: 1. Within frame O, rotate the vector v_a by euler
  //                    angle, the vector after rotation is v_b
  //                 2. Rotate frame O to a new frame O' by euler angle
  //                    the vector v_a in O' is vector v_b in O

  // mxRot = mxRotB: 1. Rotate frame O to a new frame O' by euler angle
  //                 the vector v_a in O is vector v_b in O'

  for (int i = 0; i < 3; i ++) {
    v_b[i] = 0.0;
    for (int j = 0; j < 3; j ++)
      v_b[i] += mxRot[i][j] * v_a[j];
  }
}
