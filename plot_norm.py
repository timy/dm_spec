import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("res/dm_1.dat")

idx = ["re_11", "re_22", "re_12"]

t = data[:,0]
dat = {"re_00":data[:, 1], "im_00":data[:, 2],
       "re_11":data[:, 3], "im_11":data[:, 4],
       "re_22":data[:, 5], "im_22":data[:, 6],
       "re_33":data[:, 7], "im_33":data[:, 8],
       "re_10":data[:, 9], "im_10":data[:,10],
       "re_20":data[:,11], "im_20":data[:,12],
       "re_31":data[:,13], "im_31":data[:,14],
       "re_32":data[:,15], "im_32":data[:,16],
       "re_12":data[:,17], "im_12":data[:,18],
       "re_30":data[:,19], "im_30":data[:,20] }

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

norm = dat["re_00"] + dat["re_11"] + dat["re_22"] + dat["re_33"] - 1.0
ax.plot( t, norm )
# plt.legend( handle, idx, shadow=True, loc='upper left' )
plt.xlabel('Time (fs)')
plt.ylabel('Density matrix element(s)')
plt.title('Evolution of density matrix')
#ax.set_xlim([-400,800])
ax.grid(True)
plt.savefig("fig/dm.png")
plt.show()
