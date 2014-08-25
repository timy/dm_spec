def plot_spec_1d( data, dir_idx, coo_idx, fun_idx,
                  handle, legend, ax,
                  t0=0.0, lim=[], lbl="", factor=1, padding=0.0 ):
    """
    data: the data loaded from file "ppar_1d.dat"
    dir_idx: list for directions, e.g., [ [0, 0, 1], [0, 1, 0] ]
    coo_idx: list of coordinates, e.g., [ 'z', 'z' ]
    fun_idx: list of functions applied on data e.g., [ np.real, np.abs ]
    handle: handle of plots
    legend: handle of legends
    ax: axis of the main figure
    t0: the t0 time for FFT
    lim: list, defining the time range for use, e.g., [-300, 500]
    lbl: the common label for plots of current data
    factor: the scale of current plot
    padding [fs]: the extra time padding for higher resolution with FFT
    thus "lim" can be go beyond actual data after padding
    """
    t = data[:,0]
    dat = extract_data( data )
    dt = t[1] - t[0]                                                # fs.
    if padding > 0:
        t_padding = np.arange( t[-1]+dt, t[-1]+padding+dt, dt )
        n_padding = len(t_padding)
        t = np.concatenate( [t, t_padding] )

    if lim:
        idx_range = range( *map( lambda v: (np.abs(t-v)).argmin(), lim ) )
    else:
        idx_range = range( len(t) )
    t = t[idx_range]
    idx_0 = np.abs(t - t0).argmin()

    f = np.fft.fftshift( np.fft.fftfreq( len(t), dt*41.3413733 ) ) # a.u.
    w = 2.0 * np.pi * f / 4.55941e-6                               # cm^-1

    n = len(dir_idx)
    if n != len(coo_idx) or n != len(fun_idx):
        print( "idx arrays for dir/coord/func should have same size." )
        sys.exit()
    re_im = var_re_im()
    for i in range(n):
        name = map( lambda i_re: 
                    var_key_name( dir_idx[i], coo_idx[i], i_re ), re_im )
        d = map( lambda x, y: 1j * np.complex(x, y), # Polar -> Ef
                 dat[name[0]], dat[name[1]] )
        if padding > 0:
            d = np.concatenate( [ d, np.array( [ 0.j ] * n_padding ) ] )
        dd = map( lambda idx: d[idx], idx_range )
        t1 = np.roll( t, 0-idx_0 )
        d1 = np.roll( dd, 0-idx_0 )
        ft = np.fft.fftshift( np.fft.fft( d1 ) )
        legend.append( "%s:%s%s" % (lbl, dir_idx[i], coo_idx[i]) )
        handle.append( ax.plot( w,
                                fun_idx[i]( ft ) * factor,
                                marker='.' )[0] )
        # #show the signal to transform with FFT
        # handle.append( ax.plot( t1, #  shows the data for fft
        #                         fun_idx[i]( d1 ) * factor,
        #                         marker='.' )[0] )
