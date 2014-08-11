#include "spec.h"
#include "para.h"
#include "file.h"
#include <complex>
#include <fftw3.h>
#include <cmath>
#include <cstdlib>

#define complex std::complex<double>
#define EYE complex(0.0, 1.0)

#define NX ( ps->nt )
#define DX ( ps->dt )
#define NY ( ps->n_tau )
#define DY ( ps->d_tau )

void spec_2d( complex ***pol, parameters *ps, int id )
{
    // pol: n_phase * (n_tau * nt) * n_dim
    char file_name[256];

    // spec1, spec2: NY * NX
    complex **spec1 = (complex**) malloc( NY * sizeof(complex*) );
    complex **spec2 = (complex**) malloc( NY * sizeof(complex*) );
    for (long iy = 0; iy < NY; iy ++) {
        spec1[iy] = (complex*) malloc( NX * sizeof(complex) );
        spec2[iy] = (complex*) malloc( NX * sizeof(complex) );
    }
    // temp arrays for fftw
    complex *in1 = (complex*) fftw_malloc( NX * sizeof(fftw_complex) );
    complex *ou1 = (complex*) fftw_malloc( NX * sizeof(fftw_complex) );
    complex *in2 = (complex*) fftw_malloc( NY * sizeof(fftw_complex) );
    complex *ou2 = (complex*) fftw_malloc( NY * sizeof(fftw_complex) );
    // plan for fftw
    fftw_plan p1 = fftw_plan_dft_1d( NX, 
                                     (fftw_complex*) in1,
                                     (fftw_complex*) ou1,
                                     FFTW_FORWARD, FFTW_ESTIMATE );
    fftw_plan p2 = fftw_plan_dft_1d( NY, 
                                     (fftw_complex*) in2,
                                     (fftw_complex*) ou2,
                                     FFTW_FORWARD, FFTW_ESTIMATE );

    // for (int i_dim = 0; i_dim < N_DIM; i_dim ++) {
    for (int i_dim = 2; i_dim <= 2; i_dim ++) {
        // 1st FFT over t
        for (long iy = 0; iy < NY; iy ++) { // tau
            // for (long ix = 0; ix < NX; ix ++) {// t 
            //     // in1[it] = pol[id][i_tau*(ps->nt)+it][i_dim];
            //     // due to the reversed order
            //     // EYE to convert from polarization to Efield
            //     in1[ix] = EYE * pol[id][(NY-1-iy)*NX+ix][i_dim];
            // }
            // One also needs to consider the circular shift along t
            // nn is the index of element for t = 0
            long nn = (long) ( (- ps->t0) / (ps->dt) );
            // double Tsq = (ps->t1) * (ps->t1) * 0.6 * 0.6;
            // long win_cen = (NX - nn) / 2;
            // double dtsq = (ps->dt) * (ps->dt);
            for (long ix = 0; ix < NX-nn; ix ++) {
                // double window = exp( - (ix-win_cen) * (ix-win_cen) * 
                //                      dtsq / Tsq );
                // in1[ix] = EYE * pol[id][(NY-1-iy)*NX+(ix+nn)][i_dim];
                in1[ix] = EYE * pol[id][iy*NX+(ix+nn)][i_dim];
                // in1[ix] *= window;
            }

            for (long ix = 0; ix < nn; ix ++) {
                // circular shift
                // in1[NX-nn+ix] = EYE * pol[id][(NY-1-iy)*NX+ix][i_dim];
                in1[NX-nn+ix] = 0.0; // or padding with 0
            }
            fftw_execute( p1 );

            // the "in-order" one
            // even: [0, 1, 2, 3, -2, -1]     -> [-2, -1, 0, 1, 2, 3]
            // odd:  [0, 1, 2, 3, -3, -2, -1] -> [-3, -2, -1, 0, 1, 2, 3]
            // no matter nt is odd or even...

            // ORIGINAL ONE FOR POSITIVE/NEGATIVE FREQ
            long nhalf = NX / 2;
            long ndiff = NX - nhalf;
            for (long ix = ndiff; ix < NX; ix ++)
                spec1[iy][ix-ndiff] = ou1[ix];
            for (long ix = 0; ix < ndiff; ix ++)
                spec1[iy][ix+nhalf] = ou1[ix];
            // END
            // for (long ix = 0; ix < NX; ix ++)
            //     spec1[iy][ix] = ou1[ix];
        }
        // 2nd FFT over tau
        for (long ix = 0; ix < NX; ix ++) {
            for (long iy = 0; iy < NY; iy ++)
                in2[iy] = spec1[iy][ix];
            // try reverse the tau direction
            // for (long iy = 0; iy < NY; iy ++)
            //     in2[iy] = spec1[NY-1-iy][ix];
            

            fftw_execute( p2 );

            long nhalf = NY / 2;
            long ndiff = NY - nhalf;
            for (long iy = ndiff; iy < NY; iy ++)
                spec2[iy-ndiff][ix] = ou2[iy];
            for (long iy = 0; iy < ndiff; iy ++)
                spec2[iy+nhalf][ix] = ou2[iy];
        }
        // output    
        sprintf( file_name, "res/spec_2d_%2d_%1d.dat", id, i_dim );
        FILE *file = fopen( file_name, "w" );
        double dw_y = 2.0 * M_PI / (DY * NY) / C_cm2au;
        double dw_x = 2.0 * M_PI / (DX * NX) / C_cm2au;

        // POSITIVE/NEGATIVE FREQ
        double wx_0 = - ((long)(NX / 2)) * dw_x;
        double wy_0 = - ((long)(NY / 2)) * dw_y;
        
        for (long iwy = 0; iwy < NY; iwy ++)
            for (long iwx = 0; iwx < NX; iwx ++)
                fprintf( file, "%le %le %le %le\n",
                         wy_0 + iwy * dw_y, // FIXME y!!
                         wx_0 + iwx * dw_x,
                         real( spec2[iwy][iwx] ),
                         imag( spec2[iwy][iwx] ) );
        fclose( file ); 
    }
    fftw_destroy_plan( p1 );
    fftw_destroy_plan( p2 );
    fftw_free( in1 ); fftw_free( ou1 );
    fftw_free( in2 ); fftw_free( ou2 );
    // clean
    for (long iy = 0; iy < NY; iy ++) {
        free( spec1[iy] );
        free( spec2[iy] );
    }
    free( spec1 );
    free( spec2 );
}

// -------------------------------------------------------

void spec_1d( complex ***pol, const char *name, parameters *ps )
{
    // pol: n_phase * NX * n_dim
    //char file_name[256];
    // spec1: NX
    complex *spec1 = (complex*) malloc( NX * sizeof(complex) );
    complex *in1 = (complex*) fftw_malloc( NX * sizeof(fftw_complex) );
    complex *ou1 = (complex*) fftw_malloc( NX * sizeof(fftw_complex) );
    fftw_plan p1 = fftw_plan_dft_1d( NX,
                                     (fftw_complex*) in1,
                                     (fftw_complex*) ou1,
                                     FFTW_FORWARD, FFTW_ESTIMATE );
    for (int id = 0; id < (ps->seid->n_phase); id ++) {
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
            // circular shift along t
            // nn is the index for t = 0
            long nn = long ( ( - (ps->t0)) / (ps->dt) );
            for (long ix = 0; ix < NX-nn; ix ++)
                in1[ix] = EYE * pol[id][ix+nn][i_dim];
            for (long ix = 0; ix < nn; ix ++) {
                // circular shift
                // in1[Nx-nn+ix] = EYE * pol[id][ix][i_dim]; 
                in1[NX-nn+ix] = 0.0; // or padding with 0
            }
            fftw_execute( p1 );
            
            // circular shift along w_t
            long nhalf = NX / 2;
            long ndiff = NX - nhalf;
            for (long ix = ndiff; ix < NX; ix ++)
                spec1[ix-ndiff] = ou1[ix];
            for (long ix = 0; ix < ndiff; ix ++)
                spec1[ix+nhalf] = ou1[ix];
            
            // output
            int file_idx[2] = { id, i_dim };
            open_para_file_write( para_file::SPEC_1D, NULL, ps,
                                  2, file_idx );

            // sprintf( file_name, "%s_%2d_%1d.dat", name, id, i_dim );
            //sprintf( file_name, "res/spec_1d_%2d_%1d.dat", id, i_dim );
            // FILE *file = fopen( file_name, "w" );
            double dw_x = 2.0 * M_PI / (DX * NX) / C_cm2au;
            double wx_0 = - (nhalf) * dw_x;
            for (long iwx = 0; iwx < NX; iwx ++)
                fprintf( ps->file->one[para_file::SPEC_1D]->fptr,
                         "%le %le %le\n", wx_0 + iwx * dw_x,
                         real( spec1[iwx] ), imag( spec1[iwx] ) );
                // fprintf( file, "%le %le %le\n",
                //          wx_0 + iwx * dw_x,
                //          real( spec1[iwx] ),
                //          imag( spec1[iwx] ) );
            // fclose( file ); 
            
            close_para_file( para_file::SPEC_1D, ps );
        }
    }
    // clean
    fftw_destroy_plan( p1 );
    fftw_free( in1 ); fftw_free( ou1 );
    free( spec1 );
}


void spec_1d_test( complex *pol, double dx, long nx, FILE* file, parameters *ps )
{
    complex *spec1 = (complex*) malloc( nx * sizeof(complex) );
    complex *in1 = (complex*) fftw_malloc( nx * sizeof(fftw_complex) );
    complex *ou1 = (complex*) fftw_malloc( nx * sizeof(fftw_complex) );
    fftw_plan p1 = fftw_plan_dft_1d( nx,
                                     (fftw_complex*) in1,
                                     (fftw_complex*) ou1,
                                     FFTW_FORWARD, FFTW_ESTIMATE );
    // circular shift along t, nn is the index for t = 0
    long nn = long ( ( - (ps->t0)) / (ps->dt) );
    for (long ix = 0; ix < nx-nn; ix ++)
        in1[ix] = EYE * pol[ix+nn];
    for (long ix = 0; ix < nn; ix ++) {
        // in1[nx-nn+ix] = EYE * pol[id][ix][i_dim]; 
        in1[nx-nn+ix] = 0.0; // or padding with 0
    }
    fftw_execute( p1 );
    
    // circular shift along w_t
    long nhalf = nx / 2;
    long ndiff = nx - nhalf;
    for (long ix = ndiff; ix < nx; ix ++)
        spec1[ix-ndiff] = ou1[ix];
    for (long ix = 0; ix < ndiff; ix ++)
        spec1[ix+nhalf] = ou1[ix];
            
    // output
    double dw_x = 2.0 * M_PI / (dx * nx) / C_cm2au;
    double wx_0 = - (nhalf) * dw_x;
    for (long iwx = 0; iwx < nx; iwx ++)
        fprintf( file, "%le %le %le\n",
                 wx_0 + iwx * dw_x,
                 real( spec1[iwx] ),
                 imag( spec1[iwx] ) );
    // clean
    fftw_destroy_plan( p1 );
    fftw_free( in1 ); fftw_free( ou1 );
    free( spec1 );
}
