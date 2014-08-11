import numpy as np
import sys

sys.path.append( 'pkg' )
import pymod_indx, pymod_data, pymod_plot

label = '[-1, 1, 1] in z-axis'
func = np.abs

t = np.loadtxt( "res/time.dat" )
data = np.loadtxt( "res/ppar_1d2_ 3.dat" )


handle, legend = [], []
ax = pymod_plot.plot_1d_init()

pymod_data.plot_signal_1d( t, data, func, handle, legend, ax,
                           obj='s', t0=-100, lim=[-100.0, 546], 
                           lbl=label, padding=10.0, w_shift=16000. )

pymod_plot.plot_show( ax, handle=handle, legend=legend,
                      xlabel=r"Freq. (cm$^{-1}$)",
                      ylabel=r"Signal (arb.)", 
                      title=r"Spectrum" )#,
#xlim=[-80000, 8000] )
