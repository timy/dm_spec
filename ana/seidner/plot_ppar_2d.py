import numpy as np
import sys

sys.path.append( '../../pkg' )
import pymod_indx, pymod_data, pymod_plot

dat_idx = [[0, '[-1, 1, 1] in z-axis']]
func = np.abs

time = np.loadtxt( "res/time.dat" )
grid = np.loadtxt( "res/grid_2d_ 0.dat")
data = np.loadtxt( "res/ppar_2d2_15_ 0.dat" )

# import matplotlib.pyplot as plt
# plt.plot( data[0:500,0] )
# plt.show()

# sys.exit(-1)


ax = pymod_plot.plot_1d_init()

# w_shift = [-16000, +16000] means the signal is in the fourth quadrant
# because for t, we use -16000 to move the signal to frequency origin
# and for tau, we use +16000 to move the signa to frequency origin
# so the signal has the frequency ~(+16000, -16000)
pymod_data.plot_signal_2d( time, grid, data, func, ax, padding=[0.0, 0.0],
                           w_shift=[-16000.0, +16000.0], obj='s', t0=[0,0] )
pymod_plot.plot_show( ax,
                      xlabel=r"Freq. for detection $t$ (cm$^{-1}$)",
                      ylabel=r"Freq. for coherence $\tau$ (cm$^{-1}$)",
                      title=r"2-dimensional Spectrum of Dimer System")
                      #xlim=[15000, 17000], ylim=[15400, 16600] )
                      #xlim=[0, 400], ylim=[0, 400] )



































# import matplotlib.pyplot as plt
# import numpy as np
# import sys

# n_tau = 40
# n_t =   2000

# data = np.loadtxt("res/ppar_2d0_ 3_ 0.dat")

# tau_lower, tau_upper = 0.0, 500.0
# t_lower, t_upper = -700, 500.0

# font = {
#     'family' : 'serif',
#     'weight' : 'normal',
#     'size'   : 20,}

# font_label = {
#     'weight' : 'normal',
#     'size'   : 16,}

# tau, t = data[::n_t,0], data[:n_t,1]
# #idx_tau, = np.nonzero( (tau >= tau_lower) & (tau <= tau_upper))
# #idx_t, = np.nonzero( (t >= t_lower) & (t <= t_upper))
# #idx_data = np.nonzero( (data[:,0] >= tau_lower) & \
# #                       (data[:,0] <= tau_upper) & \
# #                       (data[:,1] >= t_lower) & (data[:,1] <= t_upper) )
# dat = data[:, 2]
# #dat = np.sqrt(data[idx_data,2]**2 + data[idx_data,3]**2)
# #n_tau, n_t = np.size(idx_tau), np.size(idx_t)
# #print( "Number of elements used for display: ", np.size(idx_data) )
# print( "Number of tau: ", n_tau )
# print( "Number of t: ", n_t )
# spec = np.reshape(dat, (n_tau, n_t))
# #spec = np.log(spec)

# #extent = ( t[idx_t[0]], t[idx_t[n_t-1]], \
#            #tau[idx_tau[0]], tau[idx_tau[n_tau-1]] )
# #           tau[idx_tau[n_tau-1]], tau[idx_tau[0]] )
# origin = 'upper'
# norm = plt.cm.colors.Normalize(vmax=spec.max(), vmin=spec.min())
# im = plt.imshow(spec, cmap=plt.cm.summer, norm=norm,
#                 origin=origin)#, extent=extent)
# plt.contour(spec, 5, hold='on', colors = 'k', origin=origin)#,
#             #extent=extent)
# plt.colorbar(im)
# plt.title(r'2-dimensional polarization', fontdict=font)
# plt.xlabel(r'$t$', fontdict=font_label)
# plt.ylabel(r'$\tau$', fontdict=font_label)
# plt.grid(True)
# plt.savefig("fig/spec.svg")
# plt.show()
