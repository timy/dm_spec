import json
fileDir = open('index.json')
idxDir = json.load(fileDir)
fileDir.close()

import numpy as np
n_order, n_coo, nt = 2, 3, 960
n_dir = [len(idxDir[i]) for i in range(n_order)]
# calculate the abs value of emitting electric field (including all directions)
def calcAmplitude(i_order, i_dir):
    s = np.zeros(nt)
    for i_coo in range(n_coo):
        data = np.loadtxt( "res_%d/ppar_2d_dpl0_coo%d_%d_0.dat" % (i_order, i_coo, i_dir))
        for i in range(2):
            s += data[:,i]**2
    return np.sqrt(s)
# this function is not used, the index of the direction l= [l1, l2, l3]
def indexOfDir( idxDir, l ):
    for i_order in range(n_order):
        for index, item in enumerate(idxDir[i_order]):
            if item == l:
                return i_order, index
# calculate the largest value of the ith-order / jth-direction
maxAmp = np.array( [np.zeros(n_dir[i]) for i in range(n_order)] )
for i_order in range(n_order):
    for i_dir in range(n_dir[i_order]):
        maxAmp[i_order][i_dir] = np.amax( calcAmplitude(i_order, i_dir) )
        print( "%d_%d: %le" % (i_order, i_dir, maxAmp[i_order][i_dir]) )
# plot
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
fig, ax = plt.subplots(2)
for i_order in range(n_order):
    ax[i_order].plot(maxAmp[i_order], marker='.')
    ax[i_order].grid(True)
    ax[i_order].xaxis.set_major_locator(MaxNLocator(len(idxDir[i_order])+1))
    ax[i_order].set_xticklabels(idxDir[i_order], rotation='90')
plt.show()
