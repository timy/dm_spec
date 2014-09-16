import numpy as np
import sys

sys.path.append( '../../pkg' )
import pymod_indx, pymod_data, pymod_plot

dat_idx = [[0, '[-1, 1, 1] in z-axis']]
func = np.abs

time = np.loadtxt( "res/time.dat" )
grid = np.loadtxt( "res/grid_2d_0.dat")

i_dir = 34
data = np.loadtxt( "res/ppar_2d_dpl0_coo2_%d_0.dat" % i_dir )

ax = pymod_plot.plot_1d_init()

# w_shift = [-16000, +16000] means the signal is in the fourth quadrant
# because for t, we use -16000 to move the signal to 0-frequency
# and for tau, we use +16000 to move the signal to 0-frequency
# so the signal has the frequency ~(+16000, -16000)
# obj='p' to plot polarization, obj='s' to plot spectrum
pymod_data.plot_signal_2d( time, grid, data, func, ax, padding=[0.0, 0.0],
                           w_shift=[-16000.0, +16000.0], obj='s', t0=[0,0] )
pymod_plot.plot_show( ax,
                      xlabel=r"Freq. for detection $t$ (cm$^{-1}$)",
                      ylabel=r"Freq. for coherence $\tau$ (cm$^{-1}$)",
                      title=r"2-dimensional Spectrum of Dimer System")

                      # xlabel=r"Detection $t$ (fs)",
                      # ylabel=r"Coherence $\tau$ (fs)",
                      # title=r"2-dimenional Polarization of Dimer System")

                      #xlim=[15400, 16600], ylim=[15400, 16600] )
                      #xlim=[-400, 400], ylim=[0, 400] )
