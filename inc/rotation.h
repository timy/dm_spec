#ifndef ROTATION_H_
#define ROTATION_H_

void rotation_matrix( double phi, double theta, double psi,
                      double **mxRotB, double **mxRotF );

void rotate_transform( const double *v_a, double *v_b, double **mxRot );

#endif
