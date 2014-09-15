import json
fileDir = open('../../cfg/index.json')
idxDir = json.load(fileDir)['order']
fileDir.close()
order = ['1', '3', '5']

iOrder = 1;
idxStart = [0, 6, 44]
nCoo, nt = 3, 960
nDir = len(idxDir[order[iOrder]])

import numpy as np
# calculate the abs value of emitting electric field (including all directions)
def calcAmplitude(iDir):
    s = np.zeros(nt)
    for iCoo in range(nCoo):
        data = np.loadtxt( "res/ppar_2d_dpl0_coo%d_%d_0.dat" % (iCoo, iDir))
        for i in range(2):
            s += data[:,i]**2
    return np.sqrt(s)
# this function is not used, the index of the direction l= [l1, l2, l3]
def indexOfDir( idxDir, l ):
    # for iOrder in range(nOrder):
    for index, item in enumerate(idxDir):
        if item == l:
            return index
# calculate the largest value of the ith-order / jth-direction
# maxAmp = np.array( [np.zeros(nDir[i]) for i in range(nOrder)] )
maxAmp = np.zeros(nDir)
# for iOrder in range(nOrder):
for iDir in range(nDir):
    maxAmp[iDir] = np.amax( calcAmplitude(idxStart[iOrder]+iDir) )
    print( "%d: %le" % (iDir, maxAmp[iDir]) )
# plot
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
#fig, ax = plt.subplots(2)
fig = plt.figure()
ax = fig.add_subplot( 1, 1, 1 )
#for iOrder in range(nOrder):
ax.plot(maxAmp, marker='.')
ax.grid(True)
ax.xaxis.set_major_locator(MaxNLocator(nbins=nDir-1, integer=True))
ax.set_xticks(range(nDir))
ax.set_xticklabels(idxDir[order[iOrder]], rotation='90')
plt.show()
