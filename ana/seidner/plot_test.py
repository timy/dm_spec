import matplotlib.pyplot as plt
import numpy as np
import sys

font = {
    'family' : 'serif',
    'weight' : 'normal',
    'size'   : 20,}

font_label = {
    'weight' : 'normal',
    'size'   : 16,}

time = np.loadtxt( "res/time.dat" )
grid = np.loadtxt( "res/grid_2d_ 0.dat" )
data = np.loadtxt( "res/ppar_2d2_ 3_ 0.dat" )
t_lower, t_upper = time[0], time[-1]
s_lower, s_upper = grid[0], grid[-1]

dt = time[1] - time[0]



# w_tau, w_t = data[::n_t,0], data[:n_t,1]
# idx_w_tau, = np.nonzero( (w_tau >= w_tau_lower) & (w_tau <= w_tau_upper))
# idx_w_t, = np.nonzero( (w_t >= w_t_lower) & (w_t <= w_t_upper) )
# idx_data = np.nonzero( (data[:,0] >= w_tau_lower) & \
#                        (data[:,0] <= w_tau_upper) & \
#                        (data[:,1] >= w_t_lower) & \
#                        (data[:,1] <= w_t_upper) )
#dat = data[idx_data, 0]
#dat = data[:, 0]
dat = np.sqrt(data[:,0]**2 + data[:,1]**2)
#n_tau, n_t = np.size(idx_w_tau), np.size(idx_w_t)

n_tau, n_t = 80, 2000
spec = np.reshape(dat, (n_tau, n_t))
#spec = np.log(spec)

# extent = ( w_t[idx_w_t[0]], w_t[idx_w_t[n_t-1]], \
#            w_tau[idx_w_tau[0]], w_tau[idx_w_tau[n_tau-1]] )

extent = ( time[0], time[-1], grid[0], grid[-1] )

origin = 'lower'
norm = plt.cm.colors.Normalize(vmax=spec.max(), vmin=spec.min())
im = plt.imshow(spec, cmap=plt.cm.summer, norm=norm,
                #aspect='auto',
                origin=origin, 
                extent=extent)
plt.contour( spec, 20, hold='on', colors = 'k',
             origin=origin, 
             extent=extent)
plt.colorbar(im)
plt.title(r'2-dimensional spectroscopy', fontdict=font)
plt.xlabel(r'Freq. for $t$', fontdict=font_label)
plt.ylabel(r'Freq. for $\tau$', fontdict=font_label)
# plt.xlim(15000.0, 17000.0)
# plt.ylim(15000.0, 17000.0)
# plt.xlim(-17000.0, -15000.0)
# plt.ylim(-17000.0, -15000.0)
# plt.xlim(15000.0, 17000.0)
# plt.ylim(-17000.0, -15000.0)

plt.grid(True)
plt.savefig("fig/spec.png")
plt.show()
