#include "file.h"
#include "para.h"
#include "field.h"
#include "debug.h"
#include <complex.h>
#include <cstdio>

#define complex std::complex<double>
#define EYE complex( 0.0, 1.0 )

void output_k_and_r( long i_esmb, FILE *file, parameters *ps )
{
    // output format:
    // k1_x, k1_y, k1_z, k2_x, k2_y, k2_z, k3_x, k3_y, k3_z

    // phi, theta, psi for ensemble 1
    // r_x,  r_y,  r_z for phi_1
    // r_x,  r_y,  r_z for phi_2
    // ...
    // r_x,  r_y,  r_z for phi_44
    // \n

    // phi, theta, psi for ensemble 2
    // ...

    // output wave vector
    if (i_esmb == 0) {
        for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
                fprintf( file, "%le ", (ps->ef[i_pulse]->k0)
                         * (ps->ef[i_pulse]->kuvL[i_dim]) );
        fprintf( file, "\n\n" );
    }

    // output euler angle
    fprintf( file, "%le %le %le\n",
             ps->coord->phi, ps->coord->theta, ps->coord->psi );

    // // output spatial points: FIXME!!!! for seidner only
    // for (int i_phi = 0; i_phi < (ps->seid->n_phase); i_phi ++) {
    //     for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
    //         fprintf( file, "%le ", ps->seid->rL[i_phi][i_dim] );
    //     fprintf( file, "\n" );
    // }
    fprintf( file, "\n" );
}

void output_info( int rank, long i_esmbl, long i_tau, FILE *file_info,
                  parameters *ps )
{
    // indice
    fprintf( file_info, "rank=%d/ismb=%ld/itau=%ld/",
             rank, i_esmbl, i_tau );
    // mixture angle
    fprintf( file_info, "mixa=%le/", ps->repr->theta );
    // energy
    for (int i = 0; i < ps->n_lvl; i ++)
        fprintf( file_info, "E%d=%le/", i, ps->energy[i]/C_cm2au );
    // dipole
    for (int i = 0; i < ps->n_dpl; i ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fprintf( file_info, "D%d%c=%le/", i, 120+i_dim,
                     ps->dipole[i][i_dim] );
    // orientation
    fprintf( file_info, "phi=%f/the=%f/psi=%f/",
             ps->coord->phi,
             ps->coord->theta,
             ps->coord->psi );
    // bath
    fprintf( file_info, "g12=%le/g21=%le/G11=%le/G22=%le/G12=%le/",
             (ps->bath->g12), (ps->bath->g21),
             (ps->bath->G11), (ps->bath->G22), (ps->bath->G12) );
    fprintf( file_info, "G10=%le/G20=%le/G31=%le/G32=%le/G30=%le/",
             (ps->bath->G10), (ps->bath->G20),
             (ps->bath->G31), (ps->bath->G32), (ps->bath->G30) );
    // // Pnl coefficients: FIXME!!!! for seidner only
    // fprintf( file_info, "cpnl=" );
    // for (int i_dir = 0; i_dir < (ps->seid->n_phase); i_dir ++) {
    //     for (int i_phi = 0; i_phi < (ps->seid->n_phase); i_phi ++)
    //         fprintf( file_info, "%le,%le|",
    //                  real(ps->seid->pnl_expr[i_dir][i_phi]),
    //                  imag(ps->seid->pnl_expr[i_dir][i_phi]) );
    //     fprintf( file_info, ";" );
    // }
    fprintf( file_info, "/" );
    // EOF
    fprintf( file_info, "\n");
}

void output_mol_orient( FILE* file, parameters *ps )
{
    double phi = ps->coord->phi;
    double theta = ps->coord->theta;
    double psi = ps->coord->psi;
    // double x = sin(theta) * sin(psi);
    // double y = sin(theta) * cos(psi);
    // double z = cos(theta);

    // equivalent to mxRotF.{0,0,1}, rotation of the z-axis
    double x = sin(theta) * sin(phi);
    double y = -cos(phi) * sin(theta);
    double z = cos(theta);

    fprintf( file, "%le %le %le %le %le %le\n",
             x, y, z, phi, theta, psi );
}

void output_ef_kL( FILE* file, parameters *ps )
{
    for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++) {
        double k0 = ps->ef[i_pulse]->k0;
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fprintf( file, "%le ", k0 * ps->ef[i_pulse]->kuvL[i_dim] );
    }
}

void input_ef_kL( FILE* file, double** kL, parameters *ps )
{   // kL: n_pulse * n_dim
    for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fscanf( file, "%le ", &(kL[i_pulse][i_dim]) );
}

void output_ef_kuvL( FILE* file, parameters *ps )
{
    for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fprintf( file, "%le ", (ps->ef[i_pulse]->kuvL[i_dim]) );
    fprintf( file, "\n" );
}

void output_ef_kuvM( FILE* file, parameters *ps )
{
    for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fprintf( file, "%le ", (ps->ef[i_pulse]->kuvM[i_dim]) );
    fprintf( file, "\n" );
}

void output_ef_EuvL( FILE* file, parameters *ps )
{
    for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fprintf( file, "%le ", (ps->ef[i_pulse]->EuvL[i_dim]) );
    fprintf( file, "\n" );
}

void output_ef_EuvM( FILE* file, parameters *ps )
{
    for (int i_pulse = 0; i_pulse < ps->n_pulse; i_pulse ++)
        for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
            fprintf( file, "%le ", (ps->ef[i_pulse]->EuvM[i_dim]) );
    fprintf( file, "\n" );
}

//////////////////////////// RL ////////////////////////////////
////////////////////////////////////////////////////////////
//              Specification of file format
// column no.=3: rl components in x, y and z direction
// row no.=ps->esmb->n_emsb.
////////////////////////////////////////////////////////////

void io_rl_write( parameters* ps ) {
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        fprintf( ps->file->one[para_file::RL]->fptr, "%le ", ps->pos[i_dim] );
    fprintf( ps->file->one[para_file::RL]->fptr, "\n" );
}
void io_rl_read( parameters* ps ) {
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        fscanf( ps->file->one[para_file::RL]->fptr, "%le", &(ps->pos[i_dim]) );
}

//////////////////////////// Grid ////////////////////////////////
////////////////////////////////////////////////////////////
//              Specification of file format
// column no.=1: variable, user-defined all extra dimensions
// row no.=ps->mpic->njob. njob is decided by system
////////////////////////////////////////////////////////////

void io_grid_write( para_file::file_type type, double *s,
                    char* prefix, parameters *ps )
{
    int file_idx[1] = { (int) ps->mpic->rank };
    open_para_file_write( type, prefix, ps, 1, file_idx );
    for (long is = 0; is < ps->mpic->njob; is ++)
        fprintf( ps->file->one[type]->fptr, "%le\n", s[is] );
    close_para_file( type, ps );
}

void io_grid_read( para_file::file_type type, double *s,
                   char* prefix, parameters *ps )
{
    int file_idx[1] = { (int) ps->mpic->rank };
    open_para_file_read( type, prefix, ps, 1, file_idx );
    for (long is = 0; is < ps->mpic->njob; is ++) {
        long index = ps->mpic->idx0 + is;
        fscanf( ps->file->one[type]->fptr, "%le", &s[index] );
    }
    close_para_file( type, ps );
}

//////////////////////////// Polarization ////////////////////////////////
////////////////////////////////////////////////////////////
//              Specification of file format
// column no.=2: real and imaginary part
// row no.=(ns * nt) * n_esmb. ns includes all extra dimensions
// For the same variables, 3 files store data for 3 coordinates
////////////////////////////////////////////////////////////

void io_pol_write( FILE** file, complex** pol, parameters* ps )
{
    // here one should notice, for pullerits' method, ptot is real, one can choose
    // output only the real part to save half storage, then column no.=1
    // but when one needs post-process, one output both the real & imag parts
    long ns = ps->mpic->njob;
    for (int is = 0; is < ns; is ++)
        for (int it = 0; it < ps->nt; it ++) {
            long index = is * (ps->nt) + it;
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                fprintf( file[i_dim], "%le %le\n",
                         real(pol[index][i_dim]), imag(pol[index][i_dim]) );
                // fprintf( file[i_dim], "%le\n", real(pol[index][i_dim]) );
            }
        }
}

void io_pol_read( FILE** file, complex** pol, parameters* ps )
{
    // notice the pullerits' method one can read only a single column
    double re, im;
    long ns = ps->mpic->njob;
    long idx0 = ps->mpic->idx0;
    if ( ps->esmb->with_old == 1 )
        idx0 = 0;
    for (int is = 0; is < ns; is ++)
        for (int it = 0; it < ps->nt; it ++) {
            // notice, idx0 is used to locate the start in global array
            long index = (idx0 + is) * (ps->nt) + it;
            for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++) {
                // fscanf( file[i_dim], "%le %le", &re, &im );
                // pol[index][i_dim] = complex( re, im );
                fscanf( file[i_dim], "%le", &re );
                pol[index][i_dim] = complex( re, 0.0 );
            }
        }
}

// one needs two ps: one global one, one for each node.
// here, pol should be a global array, while ps is for the node
// ----------------------------------------
void io_pol_dir_write( para_file::file_type type, complex*** pol_2d,
                       int n_dir, char* prefix, parameters *ps )
{
    // n_dir = ps->seid->n_phase for seidner
    for (int i_dir = 0; i_dir < n_dir; i_dir ++) {
        int file_idx[2] = { i_dir, (int) ps->mpic->rank };
        open_para_file_write( type, prefix, ps, 2, file_idx );
        io_pol_write( ps->file->mul[type]->fptr, pol_2d[i_dir], ps );
        close_para_file( type, ps );
    }
}
void io_pol_dir_read( para_file::file_type type, complex*** pol_2d,
                      int n_dir, char* prefix, parameters *ps )
{
    for (int i_dir = 0; i_dir < n_dir; i_dir ++) {
        int file_idx[2] = { i_dir, (int) ps->mpic->rank };
        open_para_file_read( type, prefix, ps, 2, file_idx );
        io_pol_read( ps->file->mul[type]->fptr, pol_2d[i_dir], ps );
        close_para_file( type, ps );
    }
}
