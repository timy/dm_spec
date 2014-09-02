import numpy as np
import sys

sys.path.append('/data1/home/tianmin/test/dm2/dm_none2/pkg')
import pymod_data
import pymod_plot

ax = pymod_plot.plot_1d_init()
handle, legend = [], []
dir_idx = [ [-1, 1, 1 ], [-1, 1, 1 ], [-1, 1, 1 ] ]
coo_idx = [ 'z', 'z', 'z' ]
fun_idx = [ lambda x: np.real(x), lambda x: np.imag(x), 
            lambda x: np.abs(x) ]

home_dir = "/data1/home/tianmin/test/dm2/dm_none2/res"

data = np.loadtxt( home_dir + "/ppar_1d.dat")
pymod_data.plot_spec_1d( data, dir_idx, coo_idx, fun_idx, 
                         handle, legend, ax, 
                         t0=0, lim=[-340, 2000], lbl="T=800fs",
                         padding=1500 )


pymod_plot.plot_1d_draw( handle, legend, ax,
                         xlabel=r"Freq. (cm$^{-1}$)",
                         ylabel=r"Signal (arb.)",
                         title=r"Comparison between different $\omega_c$" )
                         #xlim=[-340,500])
                         #xlim=[15400, 16600])
#xlim=[ 15400, 16600 ])
