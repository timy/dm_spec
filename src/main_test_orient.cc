////////////////////////////////////////////////////////////
// Calculation of single molecules for 1D spectroscopy
////////////////////////////////////////////////////////////
#include <cstdio>
#include <cstdlib>
#include "complex.h"
#include "para.h"
#include "polar.h"
#include "output.h"
#include "util.h"
#include "spec.h"
#include "coord.h"

#define complex std::complex<double>

void set_para( parameters *para );

int main( int argc, char* argv[] )
{
    tic();

    parameters ps;
    set_para( &ps );

    // double euler_phi   = 2.0 * M_PI * (rand() * 1.0 / RAND_MAX);
    // double euler_theta = 1.0 * M_PI * (rand() * 1.0 / RAND_MAX);
    // double euler_psi   = 2.0 * M_PI * (rand() * 1.0 / RAND_MAX);
    complex va[3], vb[3], vc[3], vd[3], ve[3];
    va[0] = complex( 1.0, 0.0 );
    va[1] = complex( 1.0, 0.0 );
    va[2] = complex( 1.0, 0.0 );

    double euler_phi, euler_theta, euler_psi;
    euler_phi   = 0.5 * M_PI;
    euler_theta = 0.5 * M_PI;
    euler_psi   = 0.5 * M_PI;
    set_para_coord( euler_phi, euler_theta, euler_psi, &ps );
    coord_from_lab_to_mol( va, vb, &ps );

    FILE *file = fopen( "res/euler.dat", "w" );
    for (int i = 0; i < 3; i ++)
        fprintf( file, "%lf ", real( va[i] ) );
    fprintf( file, "\n" );
    for (int i = 0; i < 3; i ++)
        fprintf( file, "%lf ", real( vb[i] ) );
    fprintf( file, "\n" );
    fclose( file );

    del_para( &ps );

    tac(stdout);
    printf( "Hasta la vista!\n" );
    
    return 0;
}
