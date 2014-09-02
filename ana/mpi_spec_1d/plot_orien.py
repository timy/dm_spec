from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
from itertools import product, combinations

n_esmb = 100000

fig = plt.figure()
ax = fig.gca(projection='3d')
ax.set_aspect("equal")

data = np.loadtxt("res/euler.dat")
#draw cube
# r = [-1, 1]
# for s, e in combinations(np.array(list(product(r, r, r))), 2):
#     if np.sum(np.abs(s - e)) == (r[1] - r[0]):
#         ax.plot3D(*zip(s, e), color="b")

#draw sphere
u, v = np.mgrid[0:2*np.pi:20j, 0:np.pi:10j]
x = np.cos(u) * np.sin(v)
y = np.sin(u) * np.sin(v)
z = np.cos(v)
ax.plot_wireframe(x, y, z, color="r")

#draw a point
ax.scatter([0],[0],[0],color="g",s=100)

#draw a vector
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d

class Arrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        FancyArrowPatch.__init__(self, (0,0), (0,0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def draw(self, renderer):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, renderer.M)
        self.set_positions((xs[0],ys[0]),(xs[1],ys[1]))
        FancyArrowPatch.draw(self, renderer)

phi   = data[:,0]
theta = data[:,1]
psi   = data[:,2]

x = np.sin(theta) * np.sin(psi);
y = np.sin(theta) * np.cos(psi);
z = np.cos(theta);

for i_esmb in range(n_esmb):
    a = Arrow3D( [0, x[i_esmb]], [0, y[i_esmb]], [0, z[i_esmb]],
                 mutation_scale=20, lw=1, arrowstyle="-|>",
                 color=plt.cm.RdYlBu(i_esmb) )
    ax.add_artist(a)

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

plt.show()
