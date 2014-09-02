import matplotlib.pyplot as plt
import numpy as np
import sys

def var_direction():
    dir1 = [ [ 1, 0, 0], [ 0, 1, 0], [ 0, 0, 1],
             [-1, 1, 1], [ 1,-1, 1], [ 1, 1,-1],
             [-2, 1, 0], [ 1,-2, 0], [-2, 0, 1],
             [ 0,-2, 1], [ 1, 0,-2], [ 0, 1,-2] ]
    dir2 = map( lambda a1: map( lambda a2: -1 * a2, a1 ), dir1)
    dir3 = [ [ 3, 0, 0], [ 0, 3, 0], [ 0, 0, 3],
             [ 2, 1, 0], [ 2, 0, 1], [ 1, 2, 0],
             [ 0, 2, 1], [ 1, 0, 2], [ 0, 1, 2],
             [ 1, 1, 1] ]
    dir4 = map( lambda a1: map( lambda a2: -1 * a2, a1 ), dir3)
    # flatten to obtain a whole list of all directions
    direction = [ elem for s in [dir1, dir2, dir3, dir4] for elem in s ]
    return direction

def var_coord():
    return [ 'x', 'y', 'z' ]

def var_re_im():
    return [ "Re", "Im" ]

def var_key_name( i_dir, i_coo, i_re ):
    return "%s[%2d,%2d,%2d]%s"%(i_re, i_dir[0], i_dir[1], i_dir[2], i_coo)

def construct_var_name():
    direction, coord, re_im = var_direction(), var_coord(), var_re_im()
    name = []
    for i_dir in direction:
        for i_coo in coord:
            for i_re in re_im:
                name.append( var_key_name( i_dir, i_coo, i_re ) )
    return name

def extract_data( d ):
    name_list = construct_var_name()
    indx_list = range( 1, 1+len(name_list) )
    data_list = [ d[:, i] for i in indx_list ]
    return dict( zip( name_list, data_list ) )
    
font_title = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : 18,}
font_label = {
        'weight' : 'normal',
        'size'   : 16,}

###########################
def plot_data(data, factor=1):
    t = data[:,0]
    dat = extract_data(data)
    for i_data in data_idx:
        try:
            i_file
        except NameError:
            legend.append( "%s" % i_data )
        else:
            legend.append( "%d:%s" % (i_file, i_data) )
        handle.append( ax.plot(t, dat[i_data] * factor, marker='.')[0] )

def plot_files(file_idx, data_idx):
    for i_file in file_idx:
        data = np.loadtxt( "res/ppar_1d_%d.dat" % i_file )
        plot_data(data)

def plot_all_collection( file_name, legend, val_limit):
    # loop over all files and get the key-val map
    data = map( np.loadtxt, file_name )
    dat = map( extract_data, data )
    t = data[0][:,0]
    # find the index for val_lower and val_upper
    idx_range = range( *map( lambda v: (np.abs(t-v)).argmin(), val_limit ))
    direction, coord, re_im = var_direction(), var_coord(), var_re_im()
    for key in dat[0]:
        fig = plt.figure()
        ax = fig.add_subplot( 1, 1, 1 )
        lgd = map( lambda s: "%s (%s)" % (key, s), legend )
        handle = []
        print "Processing direction %s..." % key
        for d in dat:
            handle.append( ax.plot(t[idx_range], d[key][idx_range],
                                   marker=".")[0] )
        ax.legend( handle, lgd, shadow=True, loc='upper center' )
        ax.grid( True )
        plt.xlabel( r"Time (fs)", fontdict=font_label )
        plt.ylabel( r"Polarization (arb.)", fontdict=font_label )
        plt.title( r"Comparison of Polarization for %s in Ensemble" % key,
                   fontdict=font_title )
        plt.xlim( [ t[idx_range[0]], t[idx_range[-1]] ] )
        plt.savefig( "fig/ppar_%s.png" % key )
        plt.close()

def find_max_value( file_name, val_limit ):
    f = open( "max_val.dat", "w" )
    data = map( np.loadtxt, file_name )
    dat = map( extract_data, data )
    t = data[0][:,0]
    idx_range = range( *map( lambda v: (np.abs(t-v)).argmin(), val_limit ))
    direction, coord, re_im = var_direction(), var_coord(), var_re_im()
    for i_dir in direction:
        print "Processing direction %s..." % i_dir
        max_val = []
        for d in dat:
            s = np.zeros( len(t) )
            for i_coo in coord:
                for i_re in re_im:
                    name = var_key_name( i_dir, i_coo, i_re )
                    s += np.array(d[name]) * np.array(d[name])
            s = np.array( map( np.sqrt, s ) )
            max_val.append( np.amax( s[idx_range] ) )
        f.write( "%s\t" % i_dir )
        for v in max_val:
            f.write( "%le\t" % v )
        f.write( "\n" )
    f.close()

def plot_all_collection_total_pol( file_name, scale, legend, val_limit):
    # loop over all files and get the key-val map
    data = map( np.loadtxt, file_name )
    dat = map( extract_data, data )
    t = data[0][:,0]
    # find the index for val_lower and val_upper
    idx_range = range( *map( lambda v: (np.abs(t-v)).argmin(), val_limit ))
    direction, coord, re_im = var_direction(), var_coord(), var_re_im()
    for i_dir in direction:
        fig = plt.figure()
        ax = fig.add_subplot( 1, 1, 1 )
        lgd = map( lambda l: "%s (%s)" % (i_dir, l), legend )
        handle = []
        print "Processing direction %s..." % i_dir
        for i_file, d in enumerate(dat):
            s = np.zeros( len(t) )
            for i_coo in coord:
                for i_re in re_im:
                #for i_re in ["Re"]:
                    name = var_key_name( i_dir, i_coo, i_re )
                    s += np.array(d[name]) * np.array(d[name])
            s = np.array( map( np.sqrt, s ) )
            s *= scale[i_file]
            handle.append( ax.plot(t[idx_range], s[idx_range] )[0] )
        ax.legend( handle, lgd, shadow=True, loc='upper right' )
        ax.grid( True )
        plt.xlabel( r"Time (fs)", fontdict=font_label )
        plt.ylabel( r"Polarization (arb.)", fontdict=font_label )
        plt.title( r"Comparison of Polarization for %s in Ensemble" %
                   i_dir, fontdict=font_title )
        plt.xlim( [ t[idx_range[0]], t[idx_range[-1]] ] )
        plt.savefig( "fig/ppar_%2d_%2d_%2d.png" %
                     (i_dir[0], i_dir[1], i_dir[2]) )
        plt.close()


if __name__ == "__main__":
    home_dir = "/data1/home/tianmin/test/dm2/"
    filename = "ana/mpi_spec_1d/res/ppar_1d.dat"
    file_name = [ home_dir + "dm_euler_polar/" + filename,
                  #home_dir + "dm_euler_polar_same_dir/" + filename,
                  home_dir + "dm_2014_04_07/" + filename ]
    scale = [ 1.0, 1.0, 0.25 ]
    legend = [ "J=0", "J=300", "isotropic x 4)" ]
    #plot_all_collection( file_name, legend, [0, 300] )
    plot_all_collection_total_pol( file_name, scale, legend, [0, 500] )
    #find_max_value( file_name,  [0, 500] )
