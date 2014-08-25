import matplotlib.pyplot as plt
import numpy as np
import sys

import time as my_time

def plot_2D( signal, f, extent ):
    signal = f(signal)
    origin = 'lower'
    norm = plt.cm.colors.Normalize(vmax=signal.max(), vmin=signal.min())
    im = plt.imshow( signal, cmap=plt.cm.summer, norm=norm,
                     aspect=1, #aspect='auto',
                     origin=origin, extent=extent )
    plt.contour( signal, 20, hold='on', colors = 'k',
                 origin=origin, extent=extent )
    plt.colorbar(im)
    # plt.title(r'2-dimensional spectroscopy', fontdict=font)
    # plt.xlabel(r'Freq. for $t$', fontdict=font_label)
    # plt.ylabel(r'Freq. for $\tau$', fontdict=font_label)
    plt.xlim([-500,500])
    plt.ylim([-500,500])
    plt.grid( True )
    plt.show()

def plot_1D( signal, f ):
    signal = f(signal)
    plt.plot( signal )
    plt.grid( True )
    plt.show()




n_t, n_tau = 500, 40

time = np.loadtxt( "res/time.dat" )
grid = np.loadtxt( "res/grid_2d_ 0.dat" )
data_file = np.loadtxt( "res/ppar_2d2_ 3_ 0.dat" )

tic = my_time.time()

t_lower, t_upper = time[0], time[-1]
s_lower, s_upper = grid[0], grid[-1]

data = np.reshape( data_file[:,0] + 1j * data_file[:,1], (n_tau, n_t) )
data = -1j*data # convert from polarization to field

C_cm2au, C_fs2au = 4.55941e-6, 41.3413733
coef = C_cm2au * C_fs2au

signal = np.zeros( (n_tau, n_t), dtype=complex )

#----------------------------------------
print( "FT for each row (t)..." )
t = time
dt = t[1] - t[0]
t0 = -0.0
idx_range = range( len(t) )
t = t[idx_range]
idx_0 = np.abs(t - t0).argmin()
f = np.fft.fftshift( np.fft.fftfreq( len(t), dt*C_fs2au ) ) # a.u.
w = 2.0 * np.pi * f / C_cm2au
#t1 = np.roll( t, 0-idx_0 )

x_lower, x_upper = w[0], w[-1]

for i_tau in range(n_tau):
    data_1d = data[i_tau,:]
    data_1d = map( lambda it: 
                   data_1d[it] * np.exp( +1j*coef * 16000 * t[it] ), 
                   range( len(t) ) )
    dd = map( lambda idx: data_1d[idx], idx_range )
    d1 = np.roll( dd, 0-idx_0 )
    signal[i_tau,:] = np.fft.fftshift( np.fft.fft( d1 ) )
#----------------------------------------
print( "FT for each column (tau)..." )
t = grid
dt = t[1] - t[0]
t0 = 0.0
idx_range = range( len(t) )
t = t[idx_range]
idx_0 = np.abs(t - t0).argmin()
f = np.fft.ifftshift( np.fft.fftfreq( len(t), dt*C_fs2au ) ) # a.u.
w = 2.0 * np.pi * f / C_cm2au
#t1 = np.roll( t, 0-idx_0 )

y_lower, y_upper = w[0], w[-1]

for i_t in range(n_t):
    data_1d = signal[:,i_t]
    data_1d = map( lambda it:
                   data_1d[it] * np.exp( -1j*coef * 16000 * t[it] ),
                   range( len(t) ) )
    dd = map( lambda idx: data_1d[idx], idx_range )
    d1 = np.roll( dd, 0-idx_0 )
    signal[:,i_t] = np.fft.ifftshift( np.fft.ifft( d1 ) )

#----------------------------------------

extent = ( x_lower, x_upper, y_lower, y_upper )

print my_time.time() - tic

plot_2D( signal, np.real, extent )



#print( np.shape(data) )
#print( np.shape(signal) )
#plot_1D( data[:,2000], np.abs )
#plot_1D( data[0,:], np.real )


# dat_idx = [[0, '[-1, 1, 1] in z-axis']]
# fun_idx = [ lambda x: np.abs(x) ]

# t = np.loadtxt( "res/time.dat" )
# nt = len(t)
# data = data_file[0*nt:0*nt+nt,:]
# print(np.shape(data))


# sys.path.append( '../../pkg' )
# import pymod_indx, pymod_data, pymod_plot

# handle, legend = [], []
# ax = pymod_plot.plot_1d_init()

# pymod_data.plot_signal_1d( t, data, dat_idx, fun_idx, handle, 
#                          legend, ax, t0=-80, lim=[-50.0, 392], 
#                          lbl="", padding=.0, w_shift=16000. )

# pymod_plot.plot_draw( handle, legend, ax, 
#                          xlabel=r"Freq. (cm$^{-1}$)",
#                          ylabel=r"Signal (arb.)", 
#                          title=r"Spectrum" )#,
#                          #xlim=[-80000, 8000] )
