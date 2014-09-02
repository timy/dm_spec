import matplotlib.pyplot as plt
import numpy as np

import sys
# data = np.loadtxt("res/ptot_1d_ 0.dat")
# a = data[1, 0::3]
data = np.loadtxt("../../res/ptot_1d_19.dat")
# t = data[:,0]
# d = data[:,5]
ptot = data[2::3]

#print np.shape(ptot)

#sys.exit(-1)

font_title = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : 18,}
font_label = {
        'weight' : 'normal',
        'size'   : 16,}

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
plt.plot(ptot)
ax.grid(True)
plt.xlabel(r"Time (fs)", fontdict=font_label)
plt.ylabel(r"Polarization (arb.)", fontdict=font_label)
plt.title(r"Polarization for different directions", fontdict=font_title)
plt.savefig("fig/ppar.svg")
plt.show()
