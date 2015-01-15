import matplotlib.pyplot as plt
import numpy as np

time = np.loadtxt("res/time.dat")
data = np.loadtxt("res/ef_0.dat")

idx_ = ["x", "y", "z"]

font_title = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : 18,}
font_label = {
        'weight' : 'normal',
        'size'   : 16,}

ef = { "x":data[:, 0], "y":data[:, 1], "z":data[:, 2] }

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
handle = []
for i in idx_:
    handle.append( ax.plot(time, ef[i], marker='.')[0] )

ax.legend( handle, idx_, shadow=True, loc='upper center' )
#ax.set_xlim([-25, 5])
#ax.set_ylim([-50,0])
ax.grid(True)
plt.xlabel(r"Time (fs)", fontdict=font_label)
plt.ylabel(r"Amplitude (arb.)", fontdict=font_label)
plt.title(r"Components of electric fields", fontdict=font_title)
plt.savefig("fig/field.svg")
plt.show()
