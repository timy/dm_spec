import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("res/ode_ 0.dat")

idx = ["re_10", "re_20", "re_31", "re_32", "re_30"]
#idx = ["im_10", "im_20", "im_31", "im_32", "im_30"]
#idx = ["re_00", "re_11", "re_22", "re_12"]
#idx = ["im_00", "im_11", "im_22", "im_12"]

t = data[:,0]
dat = {"re_00":data[:, 1], "im_00":data[:, 2],
       "re_11":data[:, 3], "im_11":data[:, 4],
       "re_22":data[:, 5], "im_22":data[:, 6],
       "re_12":data[:, 7], "im_12":data[:, 8],
       "re_10":data[:, 9], "im_10":data[:,10],
       "re_20":data[:,11], "im_20":data[:,12],
       "re_31":data[:,13], "im_31":data[:,14],
       "re_32":data[:,15], "im_32":data[:,16],
       "re_30":data[:,17], "im_30":data[:,18]}

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
handle = []
for i in idx:
    handle.append( ax.plot( t, np.log10( np.abs( dat[i] ) ) )[0] )
#    handle.append( ax.plot( t, dat[i] )[0] )
#    handle.append( ax.plot( t, np.sign( dat[i] ), '.' )[0] )
plt.legend( handle, idx )
plt.xlabel('Time (fs)')
plt.ylabel('Density matrix element(s)')
plt.title('Evolution of density matrix')
#ax.set_xlim([30,55])
#ax.set_ylim([-70,-5])
#ax.set_ylim([-0.1e-30, 1e-28])
ax.grid(True)
plt.savefig("fig/dm.png")
plt.show()
