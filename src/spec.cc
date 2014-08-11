#include "complex.h"
#include "spec.h"
#include "para.h"
#include <fftw3.h>
#include <cmath>

#define complex std::complex<double>

void spec_1d( complex ***ppar, parameters *ps, int id )
{
    // ppar: n_phase * nt * n_dim
    char file_name[256];
    complex *in =
        (complex*) fftw_malloc( ps->nt * sizeof(fftw_complex) );
    complex *out =
        (complex*) fftw_malloc( ps->nt * sizeof(fftw_complex) );
    fftw_plan p = fftw_plan_dft_1d( ps->nt, (fftw_complex*) in, 
                                    (fftw_complex*) out,
                                    FFTW_BACKWARD, FFTW_ESTIMATE );
    for (int i_dim = 0; i_dim < N_DIM; i_dim ++) {
        for (long it = 0; it < ps->nt; it ++) {
            in[it] = ppar[id][it][i_dim];
        }
        fftw_execute( p );
        
        sprintf( file_name, "res/spec_1d_%2d_%1d.dat", id, i_dim );
        FILE *file = fopen( file_name, "w" );
        double dw = 2.0 * M_PI / (C_WT * (ps->dt)) / (ps->nt);
        for (long iw = 0; iw < ps->nt; iw ++)
            fprintf( file, "%le %le %le\n", iw * dw, 
                     real( out[iw] ), imag( out[iw] ) );
        fclose( file );
    }
    fftw_destroy_plan( p );     
    fftw_free( in );
    fftw_free( out );
}

void spec_1d_new( complex ***ppar, parameters *ps, int id )
{
    // ppar: n_phase * nt * n_dim
    char file_name[256];
    complex *in =
        (complex*) fftw_malloc( ps->n_t * sizeof(fftw_complex) );
    complex *out =
        (complex*) fftw_malloc( ps->n_t * sizeof(fftw_complex) );
    fftw_plan p = fftw_plan_dft_1d( ps->n_t, (fftw_complex*) in, 
                                    (fftw_complex*) out,
                                    FFTW_BACKWARD, FFTW_ESTIMATE );
    
    for (int i_dim = 0; i_dim < N_DIM; i_dim ++) {
        for (long i_t = 0; i_t < ps->n_t; i_t ++) {
            in[i_t] = ppar[id][ps->start+i_t][i_dim];
        }
        fftw_execute( p );

        sprintf( file_name, "res/spec_1d_new_%2d_%1d.dat", id, i_dim );
        FILE *file = fopen( file_name, "w" );
        double dw = 2.0 * M_PI / (C_WT * (ps->d_t)) / (ps->n_t);
        for (long iw = 0; iw < ps->n_t; iw ++)
            fprintf( file, "%le %le %le\n", iw * dw, 
                     real( out[iw] ), imag( out[iw] ) );
        fclose( file );        
    }
    fftw_destroy_plan( p );
    fftw_free( in );
    fftw_free( out );
}

// void spec_2d( complex **ppar, parameters *ps, int id )
// {
//     // ppar: n_phase -> n_tau * n_t
//     complex *out = (complex*) 
//         fftw_malloc( ps->n_t * ps->n_tau * sizeof(fftw_complex) );
//     fftw_plan p = fftw_plan_dft_2d( ps->n_tau, ps->n_t, 
//                                     (fftw_complex*) ppar[id],
//                                     (fftw_complex*) out, 
//                                     FFTW_BACKWARD, FFTW_ESTIMATE );
//     fftw_execute( p );
//     fftw_destroy_plan( p );

//     char file_name[256];
//     sprintf( file_name, "res/spec_2d_%2d.dat", id );
//     FILE *file = fopen( file_name, "w" );

//     double dw1 = 2.0 * M_PI / (C_WT * (ps->d_tau)) / (ps->n_tau);
//     double dw2 = 2.0 * M_PI / (C_WT * (ps->d_t)) / (ps->n_t);

//     for (long iw1 = 0; iw1 < ps->n_tau; iw1 ++) {
//         for (long iw2 = 0; iw2 < ps->n_t; iw2 ++) {
//             long index = iw1 * (ps->n_tau) + iw2;
//             fprintf( file, "%le %le %le %le\n",
//                      iw1 * dw1, iw2 * dw2, 
//                      real( out[index] ), imag( out[index] ) );
//         }
//     }

//     fclose( file );
//     fftw_free( out );
// }

#include <cstdlib>

void spec_2d( complex ***ppar, parameters *ps, int id )
{
    // ppar: n_phase * (n_tau * n_t) * n_dim
    char file_name[256];

    // spec1, spec2: n_tau * n_t
    complex **spec1 = (complex**)
        malloc( ps->n_tau * sizeof(complex*) );
    complex **spec2 = (complex**)
        malloc( ps->n_tau * sizeof(complex*) );
    for (long i_tau = 0; i_tau < ps->n_tau; i_tau ++) {
        spec1[i_tau] = (complex*) malloc( ps->n_t * sizeof(complex) );
        spec2[i_tau] = (complex*) malloc( ps->n_t * sizeof(complex) );
    }
    complex *in1 =
        (complex*) fftw_malloc( ps->n_t * sizeof(fftw_complex) );
    complex *ou1 =
        (complex*) fftw_malloc( ps->n_t * sizeof(fftw_complex) );
    complex *in2 =
        (complex*) fftw_malloc( ps->n_tau * sizeof(fftw_complex) );
    complex *ou2 =
        (complex*) fftw_malloc( ps->n_tau * sizeof(fftw_complex) );
    
    fftw_plan p1 = fftw_plan_dft_1d( ps->n_t,
                                     (fftw_complex*) in1,
                                     (fftw_complex*) ou1,
                                     FFTW_BACKWARD, FFTW_ESTIMATE );
    fftw_plan p2 = fftw_plan_dft_1d( ps->n_tau, 
                                     (fftw_complex*) in2,
                                     (fftw_complex*) ou2,
                                     FFTW_FORWARD, FFTW_ESTIMATE );
    
    for (int i_dim = 0; i_dim < N_DIM; i_dim ++) {
        // 1st FFT over t
        for (long i_tau = 0; i_tau < ps->n_tau; i_tau ++) {
            for (long i_t = 0; i_t < ps->n_t; i_t ++)
                in1[i_t] = ppar[id][i_tau*(ps->n_t)+i_t][i_dim];
            fftw_execute( p1 );
            for (long i_t = 0; i_t < ps->n_t; i_t ++)
                spec1[i_tau][i_t] = ou1[i_t];
        }
        // 2nd FFT over tau
        for (long i_t = 0; i_t < ps->n_t; i_t ++) {
            for (long i_tau = 0; i_tau < ps->n_tau; i_tau ++)
                in2[i_tau] = spec1[i_tau][i_t];
            fftw_execute( p2 );
            for (long i_tau = 0; i_tau < ps->n_tau; i_tau ++)
                spec2[i_tau][i_t] = ou2[i_tau];
        }
        // output
        sprintf( file_name, "res/spec_2d_%2d_%1d.dat", id, i_dim );
        FILE *file = fopen( file_name, "w" );
        double dw_tau = 2.0 * M_PI / (C_WT * (ps->d_tau)) / (ps->n_tau);
        double dw_t = 2.0 * M_PI / (C_WT * (ps->d_t)) / (ps->n_t);
        for (long iw_tau = 0; iw_tau < ps->n_tau; iw_tau ++)
            for (long iw_t = 0; iw_t < ps->n_t; iw_t ++)
                fprintf( file, "%le %le %le %le\n",
                         iw_tau * dw_tau, iw_t * dw_t, 
                         real( spec2[iw_tau][iw_t] ),
                         imag( spec2[iw_tau][iw_t] ) );
        fclose( file ); 
    }
    fftw_destroy_plan( p1 );
    fftw_destroy_plan( p2 );
    fftw_free( in1 );
    fftw_free( ou1 );
    fftw_free( in2 );
    fftw_free( ou2 );
    // clean
    for (long i_tau = 0; i_tau < ps->n_tau; i_tau ++) {
        free( spec1[i_tau] );
        free( spec2[i_tau] );
    }
    free( spec1 );
    free( spec2 );
}
