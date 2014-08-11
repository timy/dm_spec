import matplotlib.pyplot as plt
import numpy as np

def extract_data( d ):
    return {"Re[0]x":d[:, 1], "Im[0]x":d[:, 2],
            "Re[0]y":d[:, 3], "Im[0]y":d[:, 4],
            "Re[0]z":d[:, 5], "Im[0]z":d[:, 6],
            "Re[1]x":d[:, 7], "Im[1]x":d[:, 8],
            "Re[1]y":d[:, 9], "Im[1]y":d[:,10],
            "Re[1]z":d[:,11], "Im[1]z":d[:,12],
            "Re[2]x":d[:,13], "Im[2]x":d[:,14],
            "Re[2]y":d[:,15], "Im[2]y":d[:,16],
            "Re[2]z":d[:,17], "Im[2]z":d[:,18],
            "Re[3]x":d[:,19], "Im[3]x":d[:,20],
            "Re[3]y":d[:,21], "Im[3]y":d[:,22],
            "Re[3]z":d[:,23], "Im[3]z":d[:,24],
            "Re[4]x":d[:,25], "Im[4]x":d[:,26],
            "Re[4]y":d[:,27], "Im[4]y":d[:,28],
            "Re[4]z":d[:,29], "Im[4]z":d[:,30],
            "Re[5]x":d[:,31], "Im[5]x":d[:,32],
            "Re[5]y":d[:,33], "Im[5]y":d[:,34],
            "Re[5]z":d[:,35], "Im[5]z":d[:,36],
            "Re[6]x":d[:,37], "Im[6]x":d[:,38],
            "Re[6]y":d[:,39], "Im[6]y":d[:,40],
            "Re[6]z":d[:,41], "Im[6]z":d[:,42],
            "Re[7]x":d[:,43], "Im[7]x":d[:,44],
            "Re[7]y":d[:,45], "Im[7]y":d[:,46],
            "Re[7]z":d[:,47], "Im[7]z":d[:,48],
            "Re[8]x":d[:,49], "Im[8]x":d[:,50],
            "Re[8]y":d[:,51], "Im[8]y":d[:,52],
            "Re[8]z":d[:,53], "Im[8]z":d[:,54],
            "Re[9]x":d[:,55], "Im[9]x":d[:,56],
            "Re[9]y":d[:,57], "Im[9]y":d[:,58],
            "Re[9]z":d[:,59], "Im[9]z":d[:,60],
            "Re[10]x":d[:,61], "Im[10]x":d[:,62],
            "Re[10]y":d[:,63], "Im[10]y":d[:,64],
            "Re[10]z":d[:,65], "Im[10]z":d[:,66],
            "Re[11]x":d[:,67], "Im[11]x":d[:,68],
            "Re[11]y":d[:,69], "Im[11]y":d[:,70],
            "Re[11]z":d[:,71], "Im[11]z":d[:,72]}

font_title = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : 18,}
font_label = {
        'weight' : 'normal',
        'size'   : 16,}

###########################
def plot_data(data):
    t = data[:,0]
    dat = extract_data(data)
    for i_data in data_idx:
        try:
            i_file
        except NameError:
            legend.append( "%s" % i_data )
        else:
            legend.append( "%d:%s" % (i_file, i_data) )
        handle.append( ax.plot(t, dat[i_data], marker='.')[0] )

def plot_files(file_idx, data_idx):
    for i_file in file_idx:
        data = np.loadtxt( "res/ptot_1d_%d.dat" % i_file )
        plot_data(data)


fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
handle = []
legend = []

file_idx = [0, 1, 2, 3]
data_idx = ["Re[0]z"]

data = np.loadtxt( "res/ptot_1d.dat" )
plot_data(data)

#plot_files(file_idx, data_idx)

# for i_file in file_idx:
#     data = np.loadtxt( "res/ptot_1d_%d.dat" % i_file )
#     t = data[:,0]
#     dat = extract_data(data)
#     for i_data in data_idx:
#         legend.append( "%d:%s" % (i_file, i_data) )
#         handle.append( ax.plot(t, dat[i_data], marker='.')[0] )
#         #handle.append( ax.plot(t, np.log10( np.abs( dat[i] ) ) )[0] )

ax.legend( handle, legend, shadow=True, loc='upper center' )
#ax.set_xlim([-1600,1600])
ax.grid(True)
plt.xlabel(r"Time (fs)", fontdict=font_label)
plt.ylabel(r"Polarization (arb.)", fontdict=font_label)
plt.title(r"Overall polarization", fontdict=font_title)
plt.savefig("fig/ptot.svg")
plt.show()
