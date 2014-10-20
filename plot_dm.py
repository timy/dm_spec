import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("res/dm_0.dat")
field = np.loadtxt("res/ef_0.dat")
t = np.loadtxt("res/time.dat")

# Insert code generated from Mathematica here:
# don't forget to modify the Mathematica code since we have the column
# has been changed, data without time
# dat = {
#     "Rr0n0":data[:,0],
#     "Rr1n1":data[:,1],
#     "Rr2n2":data[:,2],
#     "Rr3n3":data[:,3],
#     "Rr4n4":data[:,4],
#     "Rr5n5":data[:,5],
#     "Rr6n6":data[:,6],
#     "Rr1n0":data[:,7],
#     "Ir1n0":data[:,8],
#     "Rr2n0":data[:,9],
#     "Ir2n0":data[:,10],
#     "Rr2n1":data[:,11],
#     "Ir2n1":data[:,12],
#     "Rr3n0":data[:,13],
#     "Ir3n0":data[:,14],
#     "Rr3n1":data[:,15],
#     "Ir3n1":data[:,16],
#     "Rr3n2":data[:,17],
#     "Ir3n2":data[:,18],
#     "Rr4n0":data[:,19],
#     "Ir4n0":data[:,20],
#     "Rr4n1":data[:,21],
#     "Ir4n1":data[:,22],
#     "Rr4n2":data[:,23],
#     "Ir4n2":data[:,24],
#     "Rr4n3":data[:,25],
#     "Ir4n3":data[:,26],
#     "Rr5n0":data[:,27],
#     "Ir5n0":data[:,28],
#     "Rr5n1":data[:,29],
#     "Ir5n1":data[:,30],
#     "Rr5n2":data[:,31],
#     "Ir5n2":data[:,32],
#     "Rr5n3":data[:,33],
#     "Ir5n3":data[:,34],
#     "Rr5n4":data[:,35],
#     "Ir5n4":data[:,36],
#     "Rr6n0":data[:,37],
#     "Ir6n0":data[:,38],
#     "Rr6n1":data[:,39],
#     "Ir6n1":data[:,40],
#     "Rr6n2":data[:,41],
#     "Ir6n2":data[:,42],
#     "Rr6n3":data[:,43],
#     "Ir6n3":data[:,44],
#     "Rr6n4":data[:,45],
#     "Ir6n4":data[:,46],
#     "Rr6n5":data[:,47],
#     "Ir6n5":data[:,48],
# }
dat = {
    "Rr0n0":data[:,0],
    "Rr1n1":data[:,1],
    "Rr2n2":data[:,2],
    "Rr1n0":data[:,3],
    "Ir1n0":data[:,4],
    "Rr2n0":data[:,5],
    "Ir2n0":data[:,6],
    "Rr2n1":data[:,7],
    "Ir2n1":data[:,8]
}

idx = ["Rr1n1", "Rr2n2"]

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
handle = []
# handle.append( ax.plot( t, dat["Rr0n0"]+dat["Rr1n1"]+dat["Rr2n2"] ) )
for i in idx:
    handle.append( ax.plot( t, dat[i], marker='.' )[0] )
#    handle.append( ax.plot( t, np.log10( np.abs( dat[i] ) ) )[0] )


# field_max = np.abs( np.amax( field[:,2] ) )
# ax.plot( t, field[:,2]/field_max-1.0, ".-")

plt.legend( handle, idx, shadow=True, loc='upper left' )
plt.xlabel('Time (fs)')
plt.ylabel('Density matrix element(s)')
plt.title('Evolution of density matrix')
#ax.set_xlim([-400,800])
ax.grid(True)
plt.savefig("fig/dm.png")
plt.show()
