import matplotlib.pyplot as plt
import numpy as np
import sys

n_tau = 600
n_t =   1400

data = np.loadtxt("res/spec_2d_ 4_2.dat")
w_tau_lower, w_tau_upper = -40000.0, 40000.0
w_t_lower, w_t_upper = -40000.0, +40000.0

# w_tau_lower, w_tau_upper = 15000.0, 17000.0
# w_t_lower, w_t_upper = 15000.0, 17000.0

font = {
    'family' : 'serif',
    'weight' : 'normal',
    'size'   : 20,}

font_label = {
    'weight' : 'normal',
    'size'   : 16,}

w_tau, w_t = data[::n_t,0], data[:n_t,1]
idx_w_tau, = np.nonzero( (w_tau >= w_tau_lower) & (w_tau <= w_tau_upper))
idx_w_t, = np.nonzero( (w_t >= w_t_lower) & (w_t <= w_t_upper) )
idx_data = np.nonzero( (data[:,0] >= w_tau_lower) & \
                       (data[:,0] <= w_tau_upper) & \
                       (data[:,1] >= w_t_lower) & \
                       (data[:,1] <= w_t_upper) )
dat = data[idx_data, 2]
#dat = np.sqrt(data[idx_data,2]**2 + data[idx_data,3]**2)
n_tau, n_t = np.size(idx_w_tau), np.size(idx_w_t)
print( "Number of elements used for display: ", np.size(idx_data) )
print( "Number of w_tau: ", n_tau )
print( "Number of w_t: ", n_t )
spec = np.reshape(dat, (n_tau, n_t))
#spec = np.log(spec)

extent = ( w_t[idx_w_t[0]], w_t[idx_w_t[n_t-1]], \
           w_tau[idx_w_tau[0]], w_tau[idx_w_tau[n_tau-1]] )

origin = 'upper'
norm = plt.cm.colors.Normalize(vmax=spec.max(), vmin=spec.min())
im = plt.imshow(spec, cmap=plt.cm.summer, norm=norm,
                #aspect='auto',
                origin=origin,
                extent=extent)
plt.contour(spec, 20, hold='on', colors = 'k',
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
