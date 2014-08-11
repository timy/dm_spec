import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("../res/grid.dat")
plt.plot( data[:,0] )
plt.show()
