font_title = {
    'family' : 'serif',
    'weight' : 'normal',
    'size'   : 24,}
font_label = {
    'weight' : 'normal',
    'size'   : 22,}

import matplotlib.pyplot as plt

def plot_1d_init():
    fig = plt.figure()
    ax = fig.add_subplot( 1, 1, 1 )
    return ax

def plot_show( ax, handle=[], legend=[], xlabel='x', ylabel='y', title='',
               filename='fig/figure_1.svg', show=True, grid=True,
               xlim=[], ylim=[] ):
    if legend and handle:
        ax.legend( handle, legend, shadow=True, loc='upper right' )
    ax.grid( grid )
    ax.set_title( title, fontdict=font_title )
    ax.set_xlabel( xlabel, fontdict=font_label )
    ax.set_ylabel( ylabel, fontdict=font_label )
    if xlim:
        ax.set_xlim( xlim )
    if ylim:
        ax.set_ylim( ylim )
    ax.tick_params(axis='both', which='major', labelsize=18)
    ax.tick_params(axis='both', which='minor', labelsize=16)
    ax.figure.set_size_inches(20,14)
    ax.figure.savefig( filename, dpi=100 )
    if show:
        # figManager = plt.get_current_fig_manager()
        # figManager.window.showMaximized()
        plt.show()
