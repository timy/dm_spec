import matplotlib.pyplot as plt
import numpy as np

# data = np.loadtxt("res/ptot_1d_ 0.dat")
# # a = data[1, 0::3]
#data = np.loadtxt("res/ppar_1d2_ 1.dat")
data = np.loadtxt("res/ptot_2d2_ 3.dat")
# t = data[:,0]
# d = data[:,5]
a, b = data[:,0], data[:,1]
font_title = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : 18,}
font_label = {
        'weight' : 'normal',
        'size'   : 16,}

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
plt.plot(a)
ax.grid(True)
plt.xlabel(r"Time (fs)", fontdict=font_label)
plt.ylabel(r"Polarization (arb.)", fontdict=font_label)
plt.title(r"Polarization for different directions", fontdict=font_title)
plt.savefig("fig/ppar.svg")
plt.show()
