import json
fileDir = open('../../cfg/index.json')
idxDir = json.load(fileDir)['order']['1']
fileDir.close()

# idxStart = 0, 6, 44 for 1st, 3rd and 5th order respectively
idxStart = 0
n_coo, nt = 3, 960
nDir = len(idxDir)

import numpy as np

# calculate the abs value of emitting electric field (including all directions)
def calcAmplitude(iDir):
    s = np.zeros(nt)
    for i_coo in range(n_coo):
        data = np.loadtxt( "res/ppar_2d_dpl0_coo%d_%d_0.dat" % (i_coo, iDir))
        for i in range(2):
            s += data[:,i]**2
    return np.sqrt(s)
# this function is not used, the index of the direction l= [l1, l2, l3]
def indexOfDir( idxDir, l ):
    # for i_order in range(n_order):
    for index, item in enumerate(idxDir):
        if item == l:
            return index
# calculate the largest value of the ith-order / jth-direction
# maxAmp = np.array( [np.zeros(nDir[i]) for i in range(n_order)] )
maxAmp = np.zeros(nDir)
# for i_order in range(n_order):
for iDir in range(nDir):
    maxAmp[iDir] = np.amax( calcAmplitude(idxStart+iDir) )
    print( "%d: %le" % (iDir, maxAmp[iDir]) )
# plot
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
#fig, ax = plt.subplots(2)
fig = plt.figure()
ax = fig.add_subplot( 1, 1, 1 )
#for i_order in range(n_order):
ax.plot(maxAmp, marker='.')
ax.grid(True)
ax.xaxis.set_major_locator(MaxNLocator(nbins=nDir-1, integer=True))
ax.set_xticks(range(nDir))
ax.set_xticklabels(idxDir, rotation='90')
plt.show()
