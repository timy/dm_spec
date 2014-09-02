import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("res/field_ 0.dat")

#idx_ = ["Re_z"]
idx_ = ["x", "y", "z"]

font_title = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : 18,}
font_label = {
        'weight' : 'normal',
        'size'   : 16,}

t = data[:,0]
# complex valued
# dat = {"Re_x":data[:, 1], "Im_x":data[:, 2],
#        "Re_y":data[:, 3], "Im_y":data[:, 4],
#        "Re_z":data[:, 5], "Im_z":data[:, 6]}

# real valued
dat = { "x":data[:, 1], "y":data[:, 2], "z":data[:, 3] }
       


fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
handle = []
for i in idx_:
    handle.append( ax.plot(t, dat[i], marker='.')[0] )
#    handle.append( ax.plot(t, np.log10(np.abs(dat[i])), '.')[0] )

#ax.plot(t, dat["Re_z"]*dat["Re_z"] + dat["Im_z"]+dat["Im_z"])
ax.legend( handle, idx_, shadow=True, loc='upper center' )
ax.set_xlim([-25, 5])
#ax.set_ylim([-50,0])
ax.grid(True)
plt.xlabel(r"Time (fs)", fontdict=font_label)
plt.ylabel(r"Amplitude (arb.)", fontdict=font_label)
plt.title(r"Components of electric fields", fontdict=font_title)
plt.savefig("fig/field.svg")
plt.show()
