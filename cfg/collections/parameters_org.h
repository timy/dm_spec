para->f_eom = equation_of_motion_full;

// orientation
double euler_phi   =  0.0 * M_PI;
double euler_theta =  0.0 * M_PI;
double euler_psi   =  0.0 * M_PI;

// representation
parameter_local para_local;
// add random_normal to mimic inhomogeneous broadenging
para_local.EA = 16200.0 * C_cm2au;
para_local.EB = 15800.0 * C_cm2au;
para_local.J  = 0.0 * C_cm2au;
para_local.mu_A[0] = 0.0;
para_local.mu_A[1] = 0.0;
para_local.mu_A[2] = 1.0;
para_local.mu_B[0] = 0.0;
para_local.mu_B[1] = 0.0;
para_local.mu_B[2] = 1.0;

para_local.position[0] = 1e-6;
para_local.position[1] = 1e-6;
para_local.position[2] = 1e-6;

// field
double W[N_PULSE] = { 16000.0, 16000.0, 16000.0 };
double E0[N_PULSE] = { 1e-4, 1e-4, 1e-4 };
double FWHM[N_PULSE] = { 15.0, 15.0, 15.0 };
double kdir[N_PULSE][N_DIM] = {
    { 0.0*M_PI, 0.5*M_PI, 0.0*M_PI },
    { 0.2*M_PI, 0.3*M_PI, 0.0*M_PI },
    { 0.4*M_PI, 0.5*M_PI, 0.0*M_PI } };
double Edir[N_PULSE] = { 0.*M_PI, 0.*M_PI, 0.*M_PI };
double tc[N_PULSE] = { -200.0 * C_fs2au, -100.0 * C_fs2au, 0.0 * C_fs2au };
    
// bath
para->bath->g = 0.01;
para->bath->w_cut = 50.0 * C_cm2au;
para->bath->T = C_T2au * 298.0;

para->tau = 20.0 * C_fs2au; // 500
para->T = 20.0 * C_fs2au; // 100
para->t = 100.0 * C_fs2au;
para->t0 = -400.0 * C_fs2au; // 700
para->t1 = 500.0 * C_fs2au; // 500.0
para->nt = 2000; // 1400
para->n_tau = 1; // 600

// file
set_para_file( para_file::DM,      "res/dm",      para );
set_para_file( para_file::EF,      "res/field",   para );
set_para_file( para_file::KR,      "res/kr",      para );
set_para_file( para_file::INFO,    "res/info",    para );
set_para_file( para_file::GRID_1D, "res/grid_1d", para );
set_para_file( para_file::PPAR_1D, "res/ppar_1d", para );
set_para_file( para_file::PTOT_1D, "res/ptot_1d", para );
set_para_file( para_file::SPEC_1D, "res/spec_1d", para );

set_para_file( para_file::GRID_2D, "raw/grid_2d", para );
set_para_file( para_file::PPAR_2D, "raw/ppar_2d", para );
set_para_file( para_file::PTOT_2D, "raw/ptot_2d", para );
set_para_file( para_file::SPEC_2D, "raw/spec_2d", para );


set_para_file( para_file::ORIENT,  "res/orient",  para );
set_para_file( para_file::KUVL,    "res/kuvl",    para );
set_para_file( para_file::KUVM,    "res/kuvm",    para );
set_para_file( para_file::EUVL,    "res/euvl",    para );
set_para_file( para_file::EUVM,    "res/euvm",    para );
set_para_file( para_file::RL,      "res/rl",      para );
set_para_file( para_file::RM,      "res/rm",      para );
