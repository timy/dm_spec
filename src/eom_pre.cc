parameters* ps = ((para_eom*) peom)->ps;
double**    Rr = ((para_eom*) peom)->Rr;
double**    Ir = ((para_eom*) peom)->Ir;
double**    w  = ((para_eom*) peom)->w;
double**    V  = ((para_eom*) peom)->V;
double**    R  = ((para_eom*) peom)->R;
double*     ef = ((para_eom*) peom)->ef;

// population
for (int i = 0; i < ps->n_lvl; i ++)
    Rr[i][i] = y[i];
// coherence
int index = ps->n_lvl;
for (int i = 1; i < ps->n_lvl; i ++)
    for (int j = 0; j < i; j ++) {
        Rr[i][j] = y[index++]; Ir[i][j] = y[index++];
    }

// interactions
field_real_components( ef, t, ps );
// init to zero. this should be moved to init part, since zero one no longer change
for (int i = 1; i < ps->n_lvl; i ++)
    for (int j = 0; j < i; j ++)
        V[i][j] = 0.0;

// TODO: later need to check V and dipole have the same size, including m=n or not?
for (int i_dpl = 0; i_dpl < ps->n_dpl; i_dpl ++) {
    int m = ps->dipole->index[i_dpl][0];
    int n = ps->dipole->index[i_dpl][1];
    for (int i_dim = 0; i_dim < ps->n_dim; i_dim ++)
        V[m][n] += - ps->dipole->dipole[i_dpl][i_dim] * ef[i_dim];
 }
