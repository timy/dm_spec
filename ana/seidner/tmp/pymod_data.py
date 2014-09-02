import numpy as np

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
