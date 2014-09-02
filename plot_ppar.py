import numpy as np
import sys

sys.path.append( 'pkg' )
import pymod_indx, pymod_data, pymod_plot

handle, legend = [], []
ax = pymod_plot.plot_1d_init()

func = np.real
t = np.loadtxt( "res/time.dat" )
data = np.loadtxt( "res/ppar_1d2_124_0.dat" )
label = '[ 1,-2, 2]'
pymod_data.plot_signal_1d( t, data, func, handle, legend, ax,
                           obj='p', t0=-100, lim=[-300.0, 546],
                           lbl=label, padding=10.0, w_shift=16000. )

pymod_plot.plot_show( ax, handle=handle, legend=legend,
                      xlabel=r"Time (fs)",
                      ylabel=r"Signal (arb.)",
                      title=r"Polarization in time domain" )
                      # xlabel=r"Freq. (cm$^{-1}$)",
                      # ylabel=r"Signal (arb.)",
                      # title=r"Spectrum",
                      # xlim=[-17000, -15000]

#xlim=[-80000, 8000] )
