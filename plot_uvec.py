import matplotlib.pyplot as plt
import numpy as np
import pymod_uvec as uvec

var_name = ["1x", "1y", "1z", "2x", "2y", "2z", "3x", "3y", "3z"]

file_name = ["res/kuvl.dat", "res/euvl.dat"]
color = ['k', 'b', 'g', 'm']
n_file = len( file_name )

ax = uvec.create()

def plot_vector( s1, s2, s3, i ):
    x, y, z = dat[s1], dat[s2], dat[s3]
    a = uvec.Arrow3D( [0, x], [0, y], [0, z],
                 mutation_scale=20, lw=1, arrowstyle="-|>",
                 color=color[i] )
    ax.add_artist(a)
    ax.text(x, y, z, "%c"%(s1[0]))

for i in range(n_file):
    data = np.loadtxt( file_name[i] )
    dat = dict( (k, v) for (k, v) in zip( var_name, data ) )
    plot_vector( "3x", "3y", "3z", i )
    plot_vector( "2x", "2y", "2z", i )
    plot_vector( "1x", "1y", "1z", i )

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()



