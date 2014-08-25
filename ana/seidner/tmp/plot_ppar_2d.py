import numpy as np
import matplotlib.pyplot as plt
import sys

ny, nt = 400, 2000
t_lower, t_upper = -20.0, 500.0
y_lower, y_upper = 0.0, 200.0

# grid
grid = np.loadtxt("../res/grid.dat")
y, t = grid[::nt,0], grid[:nt,1]
# data
rawData = np.loadtxt("../res/ppar_2ds_ 2_ 2.dat")

idx_y, = np.nonzero( (y >= y_lower) & (y <= y_upper) )
idx_t, = np.nonzero( (t >= t_lower) & (t <= t_upper) )
idx = np.nonzero( (grid[:,0] >= y_lower) & (grid[:,0] <= y_upper) & \
                  (grid[:,1] >= t_lower) & (grid[:,1] <= t_upper) )

dat = np.sqrt( rawData[idx, 0]**2 + rawData[idx, 1]**2 )
#dat = rawData[idx, 0]

dat_max, dat_min = np.amax(dat), np.amin(dat)
#lvl_percent = np.array([ 0.2, 0.4, 0.45, 0.48, 0.52, 0.55, 0.6, 0.8 ])
lvl_percent = np.array([ 0.04, 0.1, 0.2, 0.6])
lvl_contour = dat_min + (dat_max - dat_min) * lvl_percent
print( "contour levels: ", lvl_contour )

n_y, n_t = np.size( idx_y ), np.size( idx_t )
matData = np.reshape( dat, (n_y, n_t) )
extent = ( t[idx_t[0]], t[idx_t[-1]], y[idx_y[0]], y[idx_y[-1]] )

print("idx_t[0]: ", idx_t[0])

origin = 'lower'
norm = plt.cm.colors.Normalize( vmax=matData.max(), vmin=matData.min() )
im = plt.imshow( matData, cmap=plt.cm.summer, norm=norm, origin=origin,
                 extent=extent )
plt.contour( matData, lvl_contour, hold='on', colors = 'k', origin=origin,
             extent=extent )
plt.grid( True )
plt.savefig("fig/test.svg")
plt.show()
