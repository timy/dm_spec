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


// output general information
// used for para_init_config_write and para_init_info_write
#define print_info( file, x ) {                                    \
        fprintf( file, "%s,L%d: ", __FILE__, __LINE__ );        \
        dm_fprintf( file, #x );                                 \
        fprintf( file, " = " );                                 \
        dm_fprintf( file, x );                                  \
        fputc( '\n', file );                                    \
    }
#define display_section( file, x ) {              \
        fprintf( file, "\n%s\n", x ); }

void output_info( para_file::file_type type, parameters* ps )
{
    open_para_file_write( type, NULL, ps, 0, NULL );
    FILE* file = ps->file->one[type]->fptr;

    display_section( file, "basic" );
    print_info( file, ps->n_dim );
    print_info( file, ps->n_lvl );
    print_info( file, ps->n_dpl );
    print_info( file, ps->n_pulse );

    display_section( file, "time" );
    print_info( file, ps->nt );
    print_info( file, ps->t0/C_fs2au );
    print_info( file, ps->t1/C_fs2au );
    print_info( file, ps->dt/C_fs2au );
    print_info( file, ps->it0 );
    print_info( file, ps->time[ps->it0]/C_fs2au );

    display_section( file, "core" );
    for (int i = 0; i < ps->n_lvl; i ++)
        print_info( file, ps->energy[i]/C_cm2au );
    for (int i = 0; i < ps->n_dpl; i ++)
        for (int j = 0; j < ps->n_dim; j ++)
            print_info( file, ps->dipole[i][j] );
    for (int i = 0; i < ps->n_dim; i ++)
        print_info( file, ps->pos[i] );

    display_section( file, "bath" );
    print_info( file, ps->bath->g );
    print_info( file, ps->bath->w_cut/C_cm2au );
    print_info( file, ps->bath->T/C_T2au );
    print_info( file, ps->bath->g12 );
    print_info( file, ps->bath->g21 );
    print_info( file, ps->bath->G11 );
    print_info( file, ps->bath->G22 );
    print_info( file, ps->bath->G12 );
    print_info( file, ps->bath->G10 );
    print_info( file, ps->bath->G20 );
    print_info( file, ps->bath->G31 );
    print_info( file, ps->bath->G32 );
    print_info( file, ps->bath->G30 );

    display_section( file, "repr" );
    print_info( file, ps->repr->theta );

    display_section( file, "coord" );
    print_info( file, ps->coord->phi/M_PI );
    print_info( file, ps->coord->theta/M_PI );
    print_info( file, ps->coord->psi/M_PI );

    display_section( file, "mvar" );
    print_info( file, ps->mvar->ny );
    print_info( file, ps->mvar->y0 );
    print_info( file, ps->mvar->y1 );
    print_info( file, ps->mvar->dy );
    print_info( file, ps->mvar->dimGrid );
    print_info( file, ps->esmb->n_esmb );

    display_section( file, "mpic" );
    print_info( file, ps->mpic->njob );
    print_info( file, ps->mpic->idx0 );

    display_section( file, "efield" );
    for (int i = 0; i < ps->n_pulse; i ++) {
        print_info( file, ps->ef[i]->W/C_cm2au );
        print_info( file, ps->ef[i]->E0 );
        print_info( file, ps->ef[i]->FWHM/C_fs2au );
        print_info( file, ps->ef[i]->Edir/M_PI );
        for (int j = 0; j < ps->n_dim; j ++)
            print_info( file, ps->ef[i]->kdir[j]/M_PI );
        fprintf( file, "\n" );
    }

    display_section( file, "help" );
    print_info( file, ps->help->w_max );

    close_para_file( type, ps );
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
                fscanf( file[i_dim], "%le %le", &re, &im );
                pol[index][i_dim] = complex( re, im );
                // For seidner's method, cuz ptot is allways real, we can save half storage
                // fscanf( file[i_dim], "%le", &re );
                // pol[index][i_dim] = complex( re, 0.0 );
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
