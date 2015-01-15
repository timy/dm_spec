import matplotlib.pyplot as plt
import numpy as np
import sys

sys.path.append( './pkg' )
import pymod_dm

time = np.loadtxt("res/time.dat")
data = np.loadtxt("res/dm_0.dat")
field = np.loadtxt("res/ef_0.dat")
t = np.loadtxt("res/time.dat")

n_lvl = 3
idx = ['Rr1n1', 'Rr2n2']

dm = pymod_dm.assignDmElement( n_lvl, data )
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
handle = []
for i in idx:
    handle.append( ax.plot( time, dm[i], marker='.' )[0] )

# field_max = np.abs( np.amax( field[:,2] ) )
# ax.plot( time, field[:,2]/field_max-1.0, ".-")

plt.legend( handle, idx, shadow=True, loc='upper left' )
plt.xlabel('Time (fs)')
plt.ylabel('Density matrix element(s)')
plt.title('Evolution of density matrix')
#ax.set_xlim([-400,800])
ax.grid(True)
plt.savefig("fig/dm.png")
plt.show()
