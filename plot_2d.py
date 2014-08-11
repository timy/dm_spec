import matplotlib.pyplot as plt
import numpy as np

n_tau = 601
n_t = 601
data = np.loadtxt("res/spec_2d_ 3_2.dat")
w_tau, w_t = data[::n_t,0], data[:n_t,1]
#dat = np.sqrt(data[:,2]**2 + data[:,3]**2)
dat = data[:,2]
spec = np.reshape(dat, (n_tau, n_t))
#spec = np.log(spec)

font = {
    'family' : 'serif',
    'weight' : 'normal',
    'size'   : 20,}

font_label = {
    'weight' : 'normal',
    'size'   : 16,}

extent = ( w_t[0], w_t[n_t-1], w_tau[0], w_tau[n_tau-1] )
origin = 'lower'
norm = plt.cm.colors.Normalize(vmax=spec.max(), vmin=spec.min())
im = plt.imshow(spec, cmap=plt.cm.summer, norm=norm,
                origin=origin, extent=extent)
plt.contour(spec, 20, hold='on', colors = 'k',
            origin=origin, extent=extent)
plt.colorbar(im)
plt.title(r'2-dimensional spectroscopy', fontdict=font)
plt.xlabel(r'Freq. for $t$', fontdict=font_label)
plt.ylabel(r'Freq. for $\tau$', fontdict=font_label)
#plt.xlim(14000.0, 18000.0); plt.ylim(-2000.0, 0.0)
plt.grid(True)
plt.savefig("fig/spec.svg")
plt.show()
