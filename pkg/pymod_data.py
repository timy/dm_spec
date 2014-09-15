import numpy as np

def readDirIndex(fileName):
    import json
    fileDir = open(fileName)
    idxDir = json.load(fileDir)['order']
    fileDir.close()
    order = ['1', '3', '5']
    idxStart = [0, 6, 44]
    return order, idxDir, idxStart

def calcAmplitude(fileNameFunc, nt, nCoo, iDir):
    s = np.zeros(nt)
    for iCoo in range(nCoo):
        data = np.loadtxt( fileNameFunc(iCoo, iDir) )
        for i in range(2):
            s += data[:,i]**2
    return np.sqrt(s)
# this function is not used, the index of the direction l= [l1, l2, l3]
def indexOfDir( idxDir, l ):
    # for iOrder in range(nOrder):
    for index, item in enumerate(idxDir):
        if item == l:
            return index

def calcAmplitudeMax(fileNameFunc, nt, nDir, nCoo=3, idxStart=0):
    maxAmp = np.zeros(nDir)
    for iDir in range(nDir):
        maxAmp[iDir] = np.amax( calcAmplitude(fileNameFunc, nt, nCoo, idxStart+iDir) )
        print( "%d: %le" % (iDir, maxAmp[iDir]) )
    return maxAmp

def plotAmplitudeMax(maxAmp, idxDir, fileName):
    import matplotlib.pyplot as plt
    from matplotlib.ticker import MaxNLocator
    nDir = len(idxDir)
    fig = plt.figure()
    ax = fig.add_subplot( 1, 1, 1 )
    ax.plot(maxAmp, marker='.')
    ax.grid(True)
    ax.xaxis.set_major_locator(MaxNLocator(nbins=nDir-1, integer=True))
    ax.set_xticks(range(nDir))
    ax.set_xticklabels(idxDir, rotation='90')
    plt.savefig(fileName)
    plt.close(fig)

# def var_direction():
#     dir1 = [ [ 1, 0, 0], [ 0, 1, 0], [ 0, 0, 1],
#              [-1, 1, 1], [ 1,-1, 1], [ 1, 1,-1],
#              [-2, 1, 0], [ 1,-2, 0], [-2, 0, 1],
#              [ 0,-2, 1], [ 1, 0,-2], [ 0, 1,-2] ]
#     dir2 = map( lambda a1: map( lambda a2: -1 * a2, a1 ), dir1 )
#     dir3 = [ [ 3, 0, 0], [ 0, 3, 0], [ 0, 0, 3],
#              [ 2, 1, 0], [ 2, 0, 1], [ 1, 2, 0],
#              [ 0, 2, 1], [ 1, 0, 2], [ 0, 1, 2],
#              [ 1, 1, 1] ]
#     dir4 = map( lambda a1: map( lambda a2: -1 * a2, a1 ), dir3)
#     # flatten to obtain a whole list of all directions
#     direction = [ elem for s in [dir1, dir2, dir3, dir4] for elem in s ]
#     return direction

# def var_coord():
#     return [ 'x', 'y', 'z' ]

# def var_re_im():
#     return [ "Re", "Im" ]

# def var_key_name( i_dir, i_coo, i_re ):
#     return "%s[%2d,%2d,%2d]%s"%(i_re, i_dir[0], i_dir[1], i_dir[2], i_coo)

# def construct_var_name():
#     direction, coord, re_im = var_direction(), var_coord(), var_re_im()
#     name = []
#     for i_dir in direction:
#         for i_coo in coord:
#             for i_re in re_im:
#                 name.append( var_key_name( i_dir, i_coo, i_re ) )
#     return name

# def extract_data( d ):
#     name_list = construct_var_name()
#     indx_list = range( 1, 1+len(name_list) )
#     data_list = [ d[:, i] for i in indx_list ]
#     return dict( zip( name_list, data_list ) )

# def plot_data( data, dat_idx, handle, legend, ax, lim=[],
#                lbl="", factor=1 ):
#     t = data[:,0]
#     dat = extract_data( data )
#     if lim:
#         idx_range = range( *map( lambda v: (np.abs(t-v)).argmin(), lim ) )
#     else:
#         idx_range = range( len(t) )
#     for i_dat in dat_idx:
#         legend.append( "%s:%s" % (lbl, i_dat) )
#         handle.append( ax.plot( t[idx_range],
#                                 dat[i_dat][idx_range] * factor,
#                                 marker='.')[0] )

# def plot_data_abs( data, dir_idx, handle, legend, ax, lim=[],
#                    lbl="", factor=1 ):
#     t = data[:,0]
#     dat = extract_data( data )
#     if lim:
#         idx_range = range( *map( lambda v: (np.abs(t-v)).argmin(), lim ) )
#     else:
#         idx_range = range( len(t) )

#     coord, re_im = var_coord(), var_re_im()
#     for i_dir in dir_idx:
#         s = np.zeros( len(t) )
#         for i_coo in coord:
#             for i_re in re_im:
#                 name = var_key_name( i_dir, i_coo, i_re )
#                 s += np.array( dat[name] ) * np.array( dat[name] )
#         s = np.array( map( np.sqrt, s ) )
#         legend.append( "%s:%s" % (lbl, i_dir) )
#         handle.append( ax.plot( t[idx_range],
#                                 s[idx_range] * factor,
#                                 marker='.')[0] )

import matplotlib.pyplot as plt
# import pymod_plot
# import sys

# def plot_data_abs_all( file_name, lbl, factor=[], lim=[] ):
#     """
#     Loop over all directions. Each image shows abs results for
#     the same direction (results may come from different files)
#     """
#     data = map( np.loadtxt, file_name )
#     n_file = len( file_name )
#     direction = var_direction()
#     for i_dir in direction:
#         fig = plt.figure()
#         ax = fig.add_subplot( 1, 1, 1 )
#         handle, legend = [], []
#         print "Processing direction %s..." % i_dir
#         for i in range(n_file):
#             plot_data_abs( data[i], [i_dir], handle, legend, ax, lim,
#                            lbl[i], factor[i] )
#         fname = "fig/ppar_%2d_%2d_%2d.png" % (i_dir[0], i_dir[1], i_dir[2])
#         pymod_plot.plot_1d_draw( handle, legend, ax,
#                                  xlabel=r"Time (fs)",
#                                  ylabel=r"Polarization (arb.)",
#                                  filename=fname,
#                                  show=False )
#         #plt.xlim( [ t[idx_range[0]], t[idx_range[-1]] ] )1
#         plt.close()


def fft_1d( t, ns, data, t0=0.0, row=True, inv=False, w_shift=[],
            debug=False ):

    C_cm2au, C_fs2au = 4.55941e-6, 41.3413733
    coef = C_cm2au * C_fs2au
    if row == True:
        signal = np.zeros( (ns, len(t)), dtype=complex )
    else:
        signal = np.zeros( (len(t), ns), dtype=complex )
    # frequencies
    dt= t[1] - t[0]
    idx_0 = np.abs( t - t0 ).argmin()
    if inv == False: # don't know if this is correct..
        f = np.fft.fftshift( np.fft.fftfreq( len(t), dt*C_fs2au ) )
    else:
        f = np.fft.ifftshift( np.fft.fftfreq( len(t), dt*C_fs2au ) )

    w = 2.0 * np.pi * f / C_cm2au
    if w_shift:
        w -= w_shift
    # data
    for i in range(ns):
        if row == True:
            data_1d = data[i,:]
        else:
            data_1d = data[:,i]
        data_1d = np.squeeze( np.asarray( data_1d ) )

        # hanning window
        # data_1d = data_1d * np.hanning(len(data_1d))

        if w_shift:
            data_1d = map( lambda it: data_1d[it] *
                           np.exp( 1j * coef * w_shift * t[it] ),
                           range( len(t) ) )
        d1 = np.roll( data_1d, 0-idx_0 )
        if debug == True:
            plt.figure()
            t1 = np.roll( t, 0-idx_0 )
            plt.plot( t1, np.abs(d1), marker='.' )
        if inv == False:
            ft = np.fft.fftshift( np.fft.fft( d1 ) )
        else:
            ft = np.fft.fftshift( np.fft.ifft( d1 ) )
        if row == True:
            signal[i,:] = ft
        else:
            signal[:,i] = ft
    return w, signal


def plot_signal_1d( t, data, func, handle, legend, ax, obj=[],
                  t0=0.0, lim=[], lbl="", factor=1, padding=0.0,
                  w_shift=[] ):
    """
    plot_signal_1d is used to plot multiple (columns of) 1d signal/spec
    from a single file. If freq is designated, the FFT is conducted
    for freq domain

    data: the data load from file storing signal
    dat_idx: list of column idx and descriptions: [[3, "z-axis"], ...]
    fun_idx: list of functions to apply on data e.g., [ np.real, np.abs ]
    handle: handle of plots
    legend: handle of legends
    ax: axis of the main figure
    t0: the t0 time for FFT
    lim: list, defining the time range for use, e.g., [-300, 500]
    lbl: the common label for plots of current data
    factor: the scale of current plot
    padding [fs]: the extra time padding for higher resolution with FFT
    thus "lim" can be go beyond actual data after padding

    |++++++++++ data format ++++++++++|
    "t" has the format:
    t0 t1 t2 ... tn, should be read from file "res/time.dat"
    ------------------------------
    "data" has the format:
    Re[p(t0)] Im[p(t0)]
    Re[p(t1)] Im[p(t1)]
    ...
    Re[p(tn)] Im[p(tn)]

    * The i-th row represent signals for the i-th time point
    * Each line can be defined as a complex variable
    ------------------------------
    """
    legend.append( "%s" % lbl )
    # convert data to complex variable, and Pol -> Ef
    d = 1j * ( data[:,0] + 1j * data[:,1] )
    dt = t[1] - t[0]                                            # fs.
    # padding should be done before invoking other operations
    if padding > 0:
        t_padding = np.arange( t[-1]+dt, t[-1]+padding+dt, dt )
        n_padding = len(t_padding)
        t = np.concatenate( [t, t_padding] )
        d = np.concatenate( [ d, np.zeros( n_padding, dtype=complex ) ] )
    # find index within lim
    if lim:
        idx_range = range( *map( lambda v: (np.abs(t-v)).argmin(), lim ) )
    else:
        idx_range = range( len(t) )
    # truncated time
    tt, dd = t[idx_range], d[idx_range]
    if obj == 'p': # draw the 'polarization' to be fft transformed
        handle.append( ax.plot( tt, func( dd ) * factor, marker='.' )[0] )
        return
    dd = np.asmatrix( dd ) # shape (n,) -> (1, n)
    w, signal = fft_1d( tt, 1, dd, t0=t0, row=True, inv=False,
                        w_shift=w_shift )
    signal = np.squeeze( signal ) # shape (1, n) -> (n,)
    handle.append( ax.plot( w, func( signal ) * factor, marker='.' )[0] )

def plot_2D( signal, ax, extent ):
    origin = 'lower'
    norm = plt.cm.colors.Normalize(vmax=signal.max(), vmin=signal.min())
    im = ax.imshow( signal, cmap=plt.cm.summer, norm=norm,
                    #aspect=1 ) #aspect='auto',
                    origin=origin,
                    extent=extent )
    ax.contour( signal, 20, hold='on', colors = 'k',
                origin=origin,
                extent=extent )
    from mpl_toolkits.axes_grid1 import make_axes_locatable
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.5)
    cbar = plt.colorbar(im, cax=cax)
    cbar.ax.tick_params(labelsize=16)

import sys
def plot_signal_2d( t, s, data, func, ax, obj=[],
                    t0=[0.0, 0.0], lim=[], lbl="",
                    factor=1, padding=0.0,
                    w_shift=[] ):
    nt, ns = len(t), len(s)
    print nt, ns
    print len(data)
    print np.shape(data)
    if ( len(data) != nt*ns ) or ( np.shape(data)[1] != 2 ):
        print( "data does not have proper shape!" )
        sys.exit()
    # t_lower, t_upper, s_lower, s_upper = t[0], t[-1], s[0], s[-1]
    data = np.reshape( data[:,0] + 1j * data[:,1], (ns, nt) )
    # convert Pol -> Ef
    data *= +1j

    if obj == 'p': # draw the 'polarization' to be fft transformed
        extent = ( t[0], t[-1], s[0], s[-1])
        plot_2D( func(data), ax, extent );
        return

    x, signal = fft_1d( t, ns, data, t0=t0[0], row=True, inv=False,
                        w_shift=w_shift[0] )

    y, signal = fft_1d( s, nt, signal, t0=t0[1], row=False, inv=False,
                        w_shift=w_shift[1] )

    # the correct one for (FFT, IFFT) sequence
    # then no need to process the signal if axes is given by ( w_t, -w_tau )
    # since the signal under IFFT is automatically generated for -w_tau
    # extent = (x[0], x[-1], -y[-1], -y[0])

    # the most general results
    # the correct one for (FFT, FFT), with axes given by ( w_t, w_tau )
    # extent = (x[0], x[-1], y[0], y[-1])

    # After general (FFT, FFT), can also flip for axes ( w_t, -w_tau )
    # can be directly compare with paper
    signal = np.flipud( signal )
    extent = ( x[0], x[-1], -y[-1], -y[0] )

    plot_2D( func(signal), ax, extent )
