import matplotlib.pyplot as plt
import numpy as np
import sys

n_tau = 61
n_t = 1001
data = np.loadtxt("res/ppar_2d_ 3_2.dat")
tau_lower, tau_upper = 0.0, 600.0
t_lower, t_upper = 0.0, 600.0

font = {
    'family' : 'serif',
    'weight' : 'normal',
    'size'   : 20,}

font_label = {
    'weight' : 'normal',
    'size'   : 16,}

tau, t = data[::n_t,0], data[:n_t,1]
idx_tau, = np.nonzero( (tau >= tau_lower) & (tau <= tau_upper))
idx_t, = np.nonzero( (t >= t_lower) & (t <= t_upper))
idx_data = np.nonzero( (data[:,0] >= tau_lower) & \
                       (data[:,0] <= tau_upper) & \
                       (data[:,1] >= t_lower) & (data[:,1] <= t_upper) )
dat = data[idx_data, 2]
#dat = np.sqrt(data[idx_data,2]**2 + data[idx_data,3]**2)
n_tau, n_t = np.size(idx_tau), np.size(idx_t)
print( "Number of elements used for display: ", np.size(idx_data) )
print( "Number of tau: ", n_tau )
print( "Number of t: ", n_t )
spec = np.reshape(dat, (n_tau, n_t))
print( np.shape(spec) )
#spec = np.log(spec)

#extent = ( t[idx_t[0]], t[idx_t[n_t-1]], \
#           tau[idx_tau[0]], tau[idx_tau[n_tau-1]] )
origin = 'lower'
norm = plt.cm.colors.Normalize(vmax=spec.max(), vmin=spec.min())
#im = plt.imshow(spec, cmap=plt.cm.summer, norm=norm,
#                origin=origin, extent=extent)

print( np.shape(t[idx_t[0]:idx_t[n_t-1]]), \
       np.shape(spec[0, idx_t[0]:idx_t[n_t-1]]) )
#plt.plot(t[idx_t[0]:idx_t[n_t-1]], spec[0,idx_t[0]:idx_t[n_t-1]])
plt.plot(spec[0,:])

#plt.colorbar(im)
#plt.title(r'2-dimensional spectroscopy', fontdict=font)
#plt.xlabel(r'Freq. for $t$', fontdict=font_label)
#plt.ylabel(r'Freq. for $\tau$', fontdict=font_label)
##plt.xlim(14000.0, 18000.0)
##plt.ylim(14000.0, 18000.0)
#plt.grid(True)
#plt.savefig("fig/spec.svg")
plt.show()
