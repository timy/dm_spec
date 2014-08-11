import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import sys

n_dir, n_esmb = 44, 200

data = np.loadtxt("res/position.dat")
if (n_dir*n_esmb) != data.shape[0]:
    print "Incorrect data size from file!"
    sys.exit( 0 )

fig = plt.figure()
ax = fig.add_subplot( 111, projection='3d' )

for i_esmb in range(n_esmb):
    x = data[i_esmb:i_esmb+n_dir,0]
    y = data[i_esmb:i_esmb+n_dir,1]
    z = data[i_esmb:i_esmb+n_dir,2]
    ax.scatter(x, y, z, c=plt.cm.RdYlBu(i_esmb))

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()
