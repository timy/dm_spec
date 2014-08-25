import matplotlib.pyplot as plt
import numpy as np
import sys


n_t, n_tau = 2000, 80

time = np.loadtxt( "res/time.dat" )
grid = np.loadtxt( "res/grid_2d_ 0.dat" )
data_file = np.loadtxt( "res/ppar_2d2_ 3_ 0.dat" )
t_lower, t_upper = time[0], time[-1]
s_lower, s_upper = grid[0], grid[-1]



data = np.reshape( data_file[:,0] + 1j * data_file[:,1], (n_tau, n_t) )
#data_1d = data[20,:]
data_1d = data[:,1280]

#t = time
t = grid

dt = t[1] - t[0]
print t

C_cm2au, C_fs2au = 4.55941e-6, 41.3413733
coef = C_cm2au * C_fs2au
data_1d = map( lambda it: data_1d[it] * np.exp( 1j*coef * 16000 * t[it] ), range( len(t) ) )

# if lim:
#     idx_range = range( *map( lambda v: (np.abs(time-v)).argmin(), lim ) )
# else:

t0 = 0.0;
idx_range = range( len(t) )
t = t[idx_range]
idx_0 = np.abs(t - t0).argmin()


f = np.fft.fftshift( np.fft.fftfreq( len(t), dt*C_fs2au ) ) # a.u.
w = 2.0 * np.pi * f / C_cm2au

dd = map( lambda idx: data_1d[idx], idx_range )
t1 = np.roll( t, 0-idx_0 )
d1 = np.roll( dd, 0-idx_0 )
ft = np.fft.fftshift( np.fft.fft( d1 ) )
plt.plot( w, np.abs( ft ) * 1, marker='.' )
#plt.plot( t1, np.real(d1) * 1, marker='.' )

plt.show()


# data = np.reshape(data_file+j*data_file, (n_tau, n_t))
# data[]
