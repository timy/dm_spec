import matplotlib.pyplot as plt
import numpy as np
#from scipy.signal import argrelextrema

font_title = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : 18,}
font_label = {
        'weight' : 'normal',
        'size'   : 16,}

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

#data = np.loadtxt("res/spec_1d_ 2_ 2.dat")
data = np.loadtxt("res/spec_1d_ 3_ 2.dat")
#data = np.loadtxt("res/spec_1d_ 3_ 2.dat")
w = data[:,0]


w_lower, w_upper = -100000.0, 100000.0
idx_w, = np.nonzero( (w >= w_lower) & (w <= w_upper))

spec_re = data[:,1]
spec_im = data[:,2]

spec = spec_re**2 + spec_im**2
#spec = np.abs(spec_im)
#spec = spec_re
#max_spec = np.amax(spec)
#max_idx = argrelextrema(spec, np.greater)[0]
#lgr_idx = np.nonzero( spec > (0.1*max_spec) )
#idx = [filter(lambda x: x in max_idx, y) for y in lgr_idx][0]
#print idx
ax.plot(w[idx_w], spec[idx_w], '.-')
#ax.set_xlim([15500,16500])
# ax.plot(w, spec_re,  'r', label='Re')
# ax.plot(w, spec_im,  'b', label='Im')
# lgd = ax.legend(loc='upper center', shadow=True)

#for i in idx:
#    ax.annotate("%7.1f"%(w[i]), xy=(w[i], spec[i]), )#xytext=(w[i]+0.01, spec[i]+0.01), )

plt.xlabel( r"Wave number (cm$^{-1}$)", fontdict=font_label )
plt.ylabel( r"Signal (arb.)", fontdict=font_label )
plt.title( r"Spectroscopy" )

ax.grid(True)
plt.savefig("fig/spec_1d.png")
plt.show()
