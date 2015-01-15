def assignDmElement( n_lvl, data ):
    if n_lvl == 2:
        dm = {
            'Rr0n0': data[:,0],
            'Rr1n1': data[:,1],
            'Rr1n0': data[:,2],
            'Ir1n0': data[:,3]
        }
    elif n_lvl == 3:
        dm = {
            'Rr0n0': data[:,0],
            'Rr1n1': data[:,1],
            'Rr2n2': data[:,2],
            'Rr1n0': data[:,3],
            'Ir1n0': data[:,4],
            'Rr2n0': data[:,5],
            'Ir2n0': data[:,6],
            'Rr2n1': data[:,7],
            'Ir2n1': data[:,8]
        }
    elif n_lvl == 4:
        dm = {
            'Rr0n0': data[:,0],
            'Rr1n1': data[:,1],
            'Rr2n2': data[:,2],
            'Rr3n3': data[:,3],
            'Rr1n0': data[:,4],
            'Ir1n0': data[:,5],
            'Rr2n0': data[:,6],
            'Ir2n0': data[:,7],
            'Rr2n1': data[:,8],
            'Ir2n1': data[:,9],
            'Rr3n0': data[:,10],
            'Ir3n0': data[:,11],
            'Rr3n1': data[:,12],
            'Ir3n1': data[:,13],
            'Rr3n2': data[:,14],
            'Ir3n2': data[:,15]
        }
    elif n_lvl == 5:
        dm = {
            'Rr0n0': data[:,0],
            'Rr1n1': data[:,1],
            'Rr2n2': data[:,2],
            'Rr3n3': data[:,3],
            'Rr4n4': data[:,4],
            'Rr1n0': data[:,5],
            'Ir1n0': data[:,6],
            'Rr2n0': data[:,7],
            'Ir2n0': data[:,8],
            'Rr2n1': data[:,9],
            'Ir2n1': data[:,10],
            'Rr3n0': data[:,11],
            'Ir3n0': data[:,12],
            'Rr3n1': data[:,13],
            'Ir3n1': data[:,14],
            'Rr3n2': data[:,15],
            'Ir3n2': data[:,16],
            'Rr4n0': data[:,17],
            'Ir4n0': data[:,18],
            'Rr4n1': data[:,19],
            'Ir4n1': data[:,20],
            'Rr4n2': data[:,21],
            'Ir4n2': data[:,22],
            'Rr4n3': data[:,23],
            'Ir4n3': data[:,24]
        }
    elif n_lvl == 6:
        dm = {
            'Rr0n0': data[:,0],
            'Rr1n1': data[:,1],
            'Rr2n2': data[:,2],
            'Rr3n3': data[:,3],
            'Rr4n4': data[:,4],
            'Rr5n5': data[:,5],
            'Rr1n0': data[:,6],
            'Ir1n0': data[:,7],
            'Rr2n0': data[:,8],
            'Ir2n0': data[:,9],
            'Rr2n1': data[:,10],
            'Ir2n1': data[:,11],
            'Rr3n0': data[:,12],
            'Ir3n0': data[:,13],
            'Rr3n1': data[:,14],
            'Ir3n1': data[:,15],
            'Rr3n2': data[:,16],
            'Ir3n2': data[:,17],
            'Rr4n0': data[:,18],
            'Ir4n0': data[:,19],
            'Rr4n1': data[:,20],
            'Ir4n1': data[:,21],
            'Rr4n2': data[:,22],
            'Ir4n2': data[:,23],
            'Rr4n3': data[:,24],
            'Ir4n3': data[:,25],
            'Rr5n0': data[:,26],
            'Ir5n0': data[:,27],
            'Rr5n1': data[:,28],
            'Ir5n1': data[:,29],
            'Rr5n2': data[:,30],
            'Ir5n2': data[:,31],
            'Rr5n3': data[:,32],
            'Ir5n3': data[:,33],
            'Rr5n4': data[:,34],
            'Ir5n4': data[:,35]
        }
    else:
        raise ValueError('n_lvl should be within 2 ~ 6')
    return dm
