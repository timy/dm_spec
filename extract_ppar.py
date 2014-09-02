import numpy as np
n_node = 20

# read in k-vector, kl is (n_pulse * n_dim)
kl_data = np.loadtxt( "res/kl_ 0.dat" )
n_pulse = np.size( kl_data ) / 3
kl = kl_data.reshape( (n_pulse, 3) )

# read in position r, rl is (n_esmb * n_dim)
rl = np.loadtxt( "res/rl_ 0.dat" )
for i_node in range(1, n_node):
    rl = np.append( rl, np.loadtxt( "res/rl_%2d.dat"%i_node ), axis=0 )

# read in total polarization, ptot_x is (n_esmb * nt)
ptot_data = np.loadtxt( "res/ptot_1d_ 0.dat" )
for i_node in range(1, n_node):
    ptot_data = np.append( ptot_data, 
                           np.loadtxt( "res/ptot_1d_%2d.dat"%i_node),
                           axis=0 )
ptot_x = ptot_data[:,0::3]
ptot_y = ptot_data[:,1::3]
ptot_z = ptot_data[:,2::3]

n_esmb, nt = np.shape(ptot_x)[0], np.shape(ptot_x)[1]

# construct k-emission from indice. k_emi is (1 * n_dim)
index = np.array( [0, 0, 1] )
k_emi = np.sum( np.array( [index[i]*kl[i] for i in range(n_pulse)] ),
                axis=0 )

# construct the exp with spatial phase for the i_esmb sample

ppar_z = np.zeros( nt, dtype=complex )

for i_esmb in range(n_esmb):
    ppar_z += np.exp( -1j * np.dot( k_emi, rl[i_esmb] ) ) * ptot_z[i_esmb]

with open('res/ppar_z.dat', 'w') as f:
    for it in range(nt):
        f.write( "%le %le\n" % 
                 ( np.real(ppar_z[it]), np.imag(ppar_z[it]) ) )
