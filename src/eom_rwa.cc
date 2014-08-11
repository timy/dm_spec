// EOM with RWA

int equation_of_motion_RWA( double t, const double y[], double *dy,
                         void *para )
{
    parameters *ps = ((para_eom*) para)->ps;

    double Rs00 = y[0];
    double Rs11 = y[1];
    double Rs22 = y[2];
    double Rs33 = y[3];
    double Rs10 = y[4];
    double Is10 = y[5];
    double Rs20 = y[6];
    double Is20 = y[7];
    double Rs31 = y[8];
    double Is31 = y[9];
    double Rs32 = y[10];
    double Is32 = y[11];
    double Rs12 = y[12];
    double Is12 = y[13];
    double Rs30 = y[14];
    double Is30 = y[15];

    // -------------TEST SECTION----------
    double w10, w20, w12, w31, w32, w30;
    w10 = 15800 * C_cm2au;
    w20 = 16200 * C_cm2au;
    w31 = 16200 * C_cm2au;
    w32 = 15800 * C_cm2au;
    w12 = - 400 * C_cm2au;
    w30 = 32000 * C_cm2au;

    complex ef = field_cmplx_RWA_test( t, ((para_eom*)para)->iphi, ps );
    double Om = ps->Om;

    double RV10 = 1.0 * real( ef );
    double IV10 = 1.0 * imag( ef );
    double RV20 = RV10;
    double IV20 = IV10;
    double RV31 = RV10;
    double IV31 = IV10;
    double RV32 = RV10;
    double IV32 = IV10;
    
    double g12 = 0.0;
    double g21 = 0.0;
    double G11 = 0.0;
    double G22 = 0.0;
    double G12 = 0.0;
    double G10 = 0.0;
    double G20 = 0.0;
    double G31 = 0.0;
    double G32 = 0.0;
    double G30 = 0.0;
    // ------------------------------------

    // // rename energy gaps
    // double w10, w20, w12, w31, w32, w30;
    // w10 = (ps->energy[1] - ps->energy[0]); // a.u.
    // w20 = (ps->energy[2] - ps->energy[0]); // a.u.
    // w12 = (ps->energy[1] - ps->energy[2]); // a.u.
    // w31 = (ps->energy[3] - ps->energy[1]); // a.u.
    // w32 = (ps->energy[3] - ps->energy[2]); // a.u.
    // w30 = (ps->energy[3] - ps->energy[0]); // a.u.

    // // printf( "w10=%le\n", w30/C_cm2au );
    // // exit(-1);
    // // rename dipoles
    // double mu_10[3], mu_20[3], mu_31[3], mu_32[3];
    // for (int i = 0; i < 3; i ++) {
    //     mu_10[i] = ps->dipole[0][i];                    // a.u.
    //     mu_20[i] = ps->dipole[1][i];                    // a.u.
    //     mu_31[i] = ps->dipole[2][i];                    // a.u.
    //     mu_32[i] = ps->dipole[3][i];                    // a.u.
    // }

    // // light fields
    // complex ef[3];
    // double Om = (ps->Om);              // a.u.
    // Ef3M( t, ((para_eom*)para)->iphi, ps, ef ); // fs for t in Ef
    // double REf[3], IEf[3];
    // for (int i = 0; i < 3; i ++) {
    //     REf[i] = real( ef[i] );
    //     IEf[i] = imag( ef[i] );
    // }
    // // interactions
    // double RV10 = 0.0; double IV10 = 0.0;
    // double RV20 = 0.0; double IV20 = 0.0;
    // double RV31 = 0.0; double IV31 = 0.0;
    // double RV32 = 0.0; double IV32 = 0.0;
    // for (int i = 0; i < 3; i ++) {
    //     RV10 += mu_10[i] * REf[i];                      // a.u.
    //     IV10 += mu_10[i] * IEf[i];                      // a.u.
    //     RV20 += mu_20[i] * REf[i];                      // a.u.
    //     IV20 += mu_20[i] * IEf[i];                      // a.u.
    //     RV31 += mu_31[i] * REf[i];                      // a.u.
    //     IV31 += mu_31[i] * IEf[i];                      // a.u.
    //     RV32 += mu_32[i] * REf[i];                      // a.u.
    //     IV32 += mu_32[i] * IEf[i];                      // a.u.
    // }

    // // dissipation
    // double g12 = ps->bath->g12; // ?
    // double g21 = ps->bath->g21; // ?
    // double G11 = ps->bath->G11;
    // double G22 = ps->bath->G22;
    // double G12 = ps->bath->G12;
    // double G10 = ps->bath->G10;
    // double G20 = ps->bath->G20;
    // double G31 = ps->bath->G31;
    // double G32 = ps->bath->G32;
    // double G30 = ps->bath->G30;

    // Re s00
    dy[0] = 2.0 * (IV10*Rs10 + IV20*Rs20 - Is10*RV10 - Is20*RV20);
    
    // Re s11
    dy[1] = 2.0 * (IV31*Rs31 + Is10*RV10 - IV10*Rs10 - Is31*RV31)
        - G11*Rs11 + g12*Rs22;

    // Re s22
    dy[2] = 2.0 * (IV32*Rs32 + Is20*RV20 - IV20*Rs20 - Is32*RV32)
        - G22*Rs22 + g21*Rs11;

    // Re s33
    dy[3] = 2.0 * (Is31*RV31 + Is32*RV32 - IV31*Rs31 - IV32*Rs32);
    
    // Re s10
    dy[4] = Is10*(w10-Om) - IV10*Rs00 + IV10*Rs11 + IV20*Rs12 + IV31*Rs30 
        + Is12*RV20 - Is30*RV31 - G10*Rs10;

    // Im s10
    dy[5] = Rs10*(Om-w10) + Is12*IV20 + Is30*IV31 + Rs00*RV10 - Rs11*RV10
        - Rs12*RV20 + Rs30*RV31 - G10*Is10;

    // Re s20
    dy[6] = Is20*(w20-Om) - IV20*Rs00 + IV10*Rs12 + IV20*Rs22 + IV32*Rs30
        - Is12*RV10 - Is30*RV32 - G20*Rs20;

    // Im s20
    dy[7] = Rs20*(Om-w20) - Is12*IV10 + Is30*IV32 - Rs12*RV10 + Rs00*RV20
        - Rs22*RV20 + Rs30*RV32 - G20*Is20;

    // Re s31
    dy[8] = Is31*(w31-Om) - IV31*Rs11 - IV32*Rs12 - IV10*Rs30 + IV31*Rs33
        + Is30*RV10 + Is12*RV32 - G31*Rs31;

    // Im s31
    dy[9] = Rs31*(Om-w31) - Is30*IV10 + Is12*IV32 - Rs30*RV10 - Rs33*RV31
        + Rs11*RV31 + Rs12*RV32 - G31*Is31;

    // Re s32
    dy[10] = Is32*(w32-Om) - IV31*Rs12 - IV32*Rs22 - IV20*Rs30 + IV32*Rs33
        + Is30*RV20 - Is12*RV31 - G32*Rs32;

    // Im s32
    dy[11] = Rs32*(Om-w32) - Is30*IV20 - Is12*IV31 - Rs30*RV20 + Rs12*RV31 
        + Rs22*RV32 - G32*Is32 - Rs33*RV32;

    // Re s12
    dy[12] = - IV20*Rs10 - IV10*Rs20 + IV32*Rs31 + IV31*Rs32 + Is20*RV10
        + Is10*RV20 - Is32*RV31 - Is31*RV32 + Is12*w12 - G12*Rs12;

    // Im s12
    dy[13] = Is20*IV10 - Is10*IV20 + Is32*IV31 - Is31*IV32 + Rs20*RV10 
        - Rs10*RV20 + Rs32*RV31 - Rs31*RV32 - Rs12*w12 - G12*Is12;

    // Re s30
    dy[14] = Is30*(w30-2*Om) - IV31*Rs10 - IV32*Rs20 + IV10*Rs31 
        + IV20*Rs32 + Is31*RV10 + Is32*RV20 - Is10*RV31 - Is20*RV32
        - G30*Rs30;

    // Im s30
    dy[15] = Rs30*(2*Om-w30) + Is31*IV10 + Is32*IV20 - Is10*IV31 
        - Is20*IV32 - Rs31*RV10 - Rs32*RV20 + Rs10*RV31 + Rs20*RV32
        - G30*Is30;

    return GSL_SUCCESS;
}
