#ifndef PARA_H_
#define PARA_H_

// forward declaration of the complex type
namespace std { template<typename T> class complex; }

// constant coefficent for omega[cm^-1] * t[fs]
#define C_WT      1.88492e-4            // ( C_WT * Om[cm^-1] * t[fs] )
                                        // == C_fs2au * C_cm2au
#define C_fs2au   41.3413733            // t[a.u.] = C_fs2au*t[fs]
#define C_cm2au   4.55941e-6            // W[a.u.] = C_cm2au*W[cm^-1]
#define C_T2au    3.16682e-6            // E[a.u.] = C_KbT2au * k_B * T[K]
#define C_om2wv   1.37807e8             // k[m^-1] = C_om2wv * W[a.u.]
                                        //         == 200 * PI * W[cm^-1]

struct para_energy
{
    int* index;
    double *energy;
};

struct para_dipole
{
    int** index;
    double **dipole;
};

struct para_bath
{
    double g;
    double w_cut;
    double T;

    double g12;
    double g21;
    double G11;
    double G22;
    double G12;
    double G10;
    double G20;
    double G31;
    double G32;
    double G30;
};

struct para_repr
{
    double theta;
    enum {
        GENERIC,
        DIMER
    } type;
    void* ptrSt;
};

struct para_coord
{
    double phi;
    double theta;
    double psi;
    // n_dim * n_dim
    double **mxRotF;
    double **mxRotB;
};

struct para_ef
{   // units in a.u.

    // set by user
    double W;
    double E0;
    double FWHM;

    // Euler angles for k, and relative angle for E
    double *kdir;
    double Edir;

    double tc;
    // filled automatically
    double ph;
    double k0;
    double *kuvL;
    double *EuvL;
    double *kuvM;
    double *EuvM;
};

struct para_esmb
{
    long n_esmb;
    void* rng; // the damn gsl_rng typedef
    int with_old;
};

struct para_mvar
{
    long ny;
    double y0;
    double y1;
    double dy;
    int dimGrid;
};

struct para_mpic
{
    long size;
    long rank; // rank of the current unit
    long njob; // number of job for the current unit
    long idx0; // first index for the current unit

    enum {
        GRID,
        ESMB
    } partition;
};

struct para_node
{
    long n_esmb;
    long n_mvar;
};

typedef struct _IO_FILE FILE;

#include <map>
struct para_file {
    enum file_type {
        DM,
        EF,
        TIME,
        INFO,
        GRID_1D,
        GRID_2D,
        PPAR_1D,
        PTOT_1D,
        SPEC_1D,
        PPAR_2D,
        PTOT_2D,
        SPEC_2D,

        CONFIG,
        ORIENT,
        KUVL,
        KUVM,
        KL,
        KM,
        EUVL,
        EUVM,
        RL,
        RM,

        LOG
    };

    struct file_item {
        char* name;

        int nVarSyn;
        int* szVarSyn;
        char** lbVarSyn;
        int nPtrSyn;

        struct node {
            char* name;
            FILE* fptr;
        } **f;
    };

    std::map<file_type, file_item*> item;

    // struct one_node {
    //     int n_file;
    //     char* name;
    //     FILE* fptr;
    // };

    // struct mul_node {
    //     int n_file;
    //     char** name;
    //     FILE** fptr;
    // };

    // std::map<file_type, one_node*> one;
    // std::map<file_type, mul_node*> mul;
};

struct para_seidner
{
    int n_phase;

    // n_phase * n_dim
    double **rL;
    double **rM;

    // n_phase * n_pulse
    int **index;
    double **phiL;
    double **phiM;

    // n_phase * n_phase
    std::complex<double> **pnl_expr;

    void* rng; // the damn gsl_rng typedef
};

struct para_help
{
    double w_max;
};

struct para_pols
{
    int bPolForEachDpl;
    int n_dpl;
    int n_dim;

    enum {
        SEIDNER,
        PULLERITS
    } method_ppar;
};

struct parameters
{
    int n_dim;
    int n_lvl;
    int n_dpl;
    int n_eom;

    int n_pulse;

    double *pos;         // position in lab frame

    double *time;

    double tau;
    double T;
    double t;
    double t0;
    double t1;
    double dt;
    long nt;
    long it0;
    double d_t;
    long n_t;

    char dirBase[1024];

    // sub-systems
    struct para_energy *energy;
    struct para_dipole *dipole;
    struct para_repr *repr;
    struct para_bath *bath;
    struct para_coord *coord;
    struct para_ef **ef;
    struct para_esmb *esmb;
    struct para_mvar *mvar;
    struct para_mpic *mpic;
    struct para_node *node;
    struct para_file *file;
    struct para_seidner *seid;
    struct para_help *help;
    struct para_pols *pols;

    // function pointer to EOM
    int (*f_eom)( double, const double*, double*, void* );
};

void para_ini( struct parameters *ps, const char* dirBase,
               long size=1, long rank=0 );
void para_del( struct parameters *ps );

#endif
