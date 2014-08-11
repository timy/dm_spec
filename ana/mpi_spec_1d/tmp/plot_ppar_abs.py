import numpy as np
import sys

sys.path.append('/data1/home/tianmin/test/dm2/dm_none2/pkg')
import pymod_data
import pymod_plot

# ax = pymod_plot.plot_1d_init()

# handle, legend = [], []

# dir_idx = [ [-1, 1, 1] ]

# data = np.loadtxt( "../res/ppar_1d.dat" )
# pymod_data.plot_data_abs( data, dir_idx,
#                           handle, legend, ax,
#                           lim=[0, 500],
#                           lbl="P(new)",
#                           factor=1.0 )

# data = np.loadtxt( "/data1/home/tianmin/test/dm2/dm_euler_polar/ana/mpi_spec_1d/res/ppar_1d.dat" )
# pymod_data.plot_data_abs( data, dir_idx,
#                           handle, legend, ax,
#                           lbl="P(old)",
#                           factor=1.0 )

# pymod_plot.plot_1d_draw( handle, legend, ax,
#                          r"Time (fs)", r"Polarization (arb.)" )

home_dir = "/data1/home/tianmin/test/dm2/"
filename = "ana/mpi_spec_1d/res/ppar_1d.dat"
file_name = [ home_dir + "dm_euler_polar/" + filename,
              home_dir + "dm_2014_04_07/" + filename ]
lbl = [ "J=0", "J=300" ]
factor = [ 1.0, 1.0 ]
lim = [ -300, 500 ]
pymod_data.plot_data_abs_all( file_name, lbl, factor=factor, lim=lim )
