import matplotlib.pyplot as plt
import numpy as np

t = np.loadtxt( "../../res/time.dat" )
data = np.loadtxt("res/ppar_1d_0_0.dat")
a = data[:,4]

font_title = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : 18,}
font_label = {
        'weight' : 'normal',
        'size'   : 16,}

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
plt.plot( t, a, marker='.' )
ax.grid( True )
plt.xlabel(r"Time (fs)", fontdict=font_label)
plt.ylabel(r"Polarization (arb.)", fontdict=font_label)
plt.title(r"Polarization for different directions", fontdict=font_title)
plt.savefig("fig/ppar.svg")
plt.show()
