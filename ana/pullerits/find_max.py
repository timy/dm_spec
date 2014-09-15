import json
fileDir = open('../../cfg/index.json')
idxDir = json.load(fileDir)['order']
fileDir.close()
order = ['1', '3', '5'];

idxStart = [0, 6, 44]
nOrder, nCoo, nt = 2, 3, 960
nDir = [len(idxDir[order[i]]) for i in range(nOrder)]

import numpy as np
# calculate the abs value of emitting electric field (including all directions)
def calcAmplitude(iOrder, iDir):
    s = np.zeros(nt)
    for iCoo in range(nCoo):
        data = np.loadtxt( "res_%d/ppar_2d_dpl0_coo%d_%d_0.dat" % (iOrder, iCoo, iDir))
        for i in range(2):
            s += data[:,i]**2
    return np.sqrt(s)
# this function is not used, the index of the direction l= [l1, l2, l3]
def indexOfDir( idxDir, l ):
    for iOrder in range(nOrder):
         for index, item in enumerate(idxDir[order[iOrder]]):
            if item == l:
                return iOrder, index
# calculate the largest value of the ith-order / jth-direction
maxAmp = np.array( [np.zeros(nDir[i]) for i in range(nOrder)] )
for iOrder in range(nOrder):
    for iDir in range(nDir[iOrder]):
        maxAmp[iOrder][iDir] = np.amax( calcAmplitude(iOrder, iDir) )
        print( "%d_%d: %le" % (iOrder, iDir, maxAmp[iOrder][iDir]) )
# plot
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
fig, ax = plt.subplots(2)
for iOrder in range(nOrder):
    ax[iOrder].plot(maxAmp[iOrder], marker='.')
    ax[iOrder].grid(True)
    ax[iOrder].xaxis.set_major_locator(MaxNLocator(nDir[iOrder]-1))
    ax[iOrder].set_xticks(range(nDir[iOrder]))
    ax[iOrder].set_xticklabels(idxDir[order[iOrder]], rotation='90')
plt.show()
