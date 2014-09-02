import numpy as np
import sys

sys.path.append('/data1/home/tianmin/test/dm2/dm_2014_04_04/pkg')
import pymod_data
import pymod_plot

ax = pymod_plot.plot_1d_init()

handle, legend = [], []

data_idx = ["Re[-1, 1, 1]z"]

data = np.loadtxt( "/data1/home/tianmin/test/dm2/dm_euler_polar/ana/mpi_spec_1d/res/ppar_1d.dat" )
pymod_data.plot_data( data, data_idx,
                      handle, legend, ax,
                      lbl="P(Isotropic)",
                      factor=1.0 )

data = np.loadtxt( "../res/ppar_1d.dat" )
pymod_data.plot_data( data, data_idx,
                      handle, legend, ax, lim=[0, 500],
                      lbl="P(Istropic with finer grid)",
                      factor=1.0 )

pymod_plot.plot_1d_draw( handle, legend, ax,
                         r"Time (fs)", r"Polarization (arb.)" )
