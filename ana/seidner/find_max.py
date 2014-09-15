import sys
sys.path.append( '../../pkg' )
import pymod_data

nOrder, nt = 3, 960
order, idxDir, idxStart = pymod_data.readDirIndex('../../cfg/index.json')

for iOrder in range(nOrder):
    print( "order %s:" % order[iOrder] )
    nDir = len(idxDir[order[iOrder]])
    maxAmp = pymod_data.calcAmplitudeMax(
        lambda iCoo, iDir: "res/ppar_2d_dpl0_coo%d_%d_0.dat" % (iCoo, iDir),
        nt, nDir, nCoo=3, idxStart=idxStart[iOrder])
    pymod_data.plotAmplitudeMax(
        maxAmp, idxDir[order[iOrder]], "fig/order_%s.png" % order[iOrder])
