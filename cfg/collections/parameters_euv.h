para->f_eom = equation_of_motion_7lv;

// orientation
double euler_phi   =  0.0 * M_PI;
double euler_theta =  0.0 * M_PI;
double euler_psi   =  0.0 * M_PI;

// representation
parameter_local para_local;   

para_local.E[0] = 0.;
para_local.E[1] = 0.114476;
para_local.E[2] = 0.19261;
para_local.E[3] = 0.20908;
para_local.E[4] = 0.226833;
para_local.E[5] = 0.26166;
para_local.E[6] = 0.265455;

para_local.mu[0][0] = 0.; // 1->0
para_local.mu[0][1] = 0.; // 1->0
para_local.mu[0][2] = -1.3942; // 1->0
para_local.mu[1][0] = 0.; // 2->0
para_local.mu[1][1] = 0.; // 2->0
para_local.mu[1][2] = -0.475482; // 2->0
para_local.mu[2][0] = 0.; // 2->1
para_local.mu[2][1] = 0.; // 2->1
para_local.mu[2][2] = 0.966763; // 2->1
para_local.mu[3][0] = 0.; // 3->0
para_local.mu[3][1] = 0.; // 3->0
para_local.mu[3][2] = -0.140825; // 3->0
para_local.mu[4][0] = 0.; // 3->1
para_local.mu[4][1] = 0.; // 3->1
para_local.mu[4][2] = -0.509177; // 3->1
para_local.mu[5][0] = 0.; // 3->2
para_local.mu[5][1] = 0.; // 3->2
para_local.mu[5][2] = 5.84586; // 3->2
para_local.mu[6][0] = 0.; // 4->0
para_local.mu[6][1] = 0.; // 4->0
para_local.mu[6][2] = 0.670544; // 4->0
para_local.mu[7][0] = 0.; // 4->1
para_local.mu[7][1] = 0.; // 4->1
para_local.mu[7][2] = -1.94464; // 4->1
para_local.mu[8][0] = 0.; // 4->2
para_local.mu[8][1] = 0.; // 4->2
para_local.mu[8][2] = -1.25936; // 4->2
para_local.mu[9][0] = 0.; // 4->3
para_local.mu[9][1] = 0.; // 4->3
para_local.mu[9][2] = -2.98942; // 4->3
para_local.mu[10][0] = 0.; // 5->0
para_local.mu[10][1] = 0.; // 5->0
para_local.mu[10][2] = 0.295936; // 5->0
para_local.mu[11][0] = 0.; // 5->1
para_local.mu[11][1] = 0.; // 5->1
para_local.mu[11][2] = -0.938856; // 5->1
para_local.mu[12][0] = 0.; // 5->2
para_local.mu[12][1] = 0.; // 5->2
para_local.mu[12][2] = -0.264498; // 5->2
para_local.mu[13][0] = 0.; // 5->3
para_local.mu[13][1] = 0.; // 5->3
para_local.mu[13][2] = 2.63094; // 5->3
para_local.mu[14][0] = 0.; // 5->4
para_local.mu[14][1] = 0.; // 5->4
para_local.mu[14][2] = -1.47464; // 5->4
para_local.mu[15][0] = 0.; // 6->0
para_local.mu[15][1] = 0.; // 6->0
para_local.mu[15][2] = 0.888229; // 6->0
para_local.mu[16][0] = 0.; // 6->1
para_local.mu[16][1] = 0.; // 6->1
para_local.mu[16][2] = -0.721741; // 6->1
para_local.mu[17][0] = 0.; // 6->2
para_local.mu[17][1] = 0.; // 6->2
para_local.mu[17][2] = 0.451707; // 6->2
para_local.mu[18][0] = 0.; // 6->3
para_local.mu[18][1] = 0.; // 6->3
para_local.mu[18][2] = 0.573866; // 6->3
para_local.mu[19][0] = 0.; // 6->4
para_local.mu[19][1] = 0.; // 6->4
para_local.mu[19][2] = -0.347169; // 6->4
para_local.mu[20][0] = 0.; // 6->5
para_local.mu[20][1] = 0.; // 6->5
para_local.mu[20][2] = 2.88176; // 6->5

para_local.position[0] = 1e-6;
para_local.position[1] = 1e-6;
para_local.position[2] = 1e-6;

// field
double W[N_PULSE] = { 25107.6, 25107.6 }; //cm^-1 -> 800nm, 124nm
double E0[N_PULSE] = { 1e-4, 1e-4 };
double FWHM[N_PULSE] = { 2.0, 1.0 };     // fs
double kdir[N_PULSE][N_DIM] = {
    { 0.2*M_PI, 0.3*M_PI, 0.0*M_PI },
    { 0.4*M_PI, 0.5*M_PI, 0.0*M_PI } };
double Edir[N_PULSE] = { 0.*M_PI, 0.*M_PI };
double tc[N_PULSE] = { -20.0 * C_fs2au, 0.0 };
 
// bath
para->bath->g = 0.00;
para->bath->w_cut = 50.0 * C_cm2au;
para->bath->T = C_T2au * 298.0;

para->tau = 20.0 * C_fs2au; // 500
para->T = 20.0 * C_fs2au; // 100
para->t = 100.0 * C_fs2au;
para->t0 = -50.0 * C_fs2au; // 700
para->t1 = 100.0 * C_fs2au; // 500.0
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
