import numpy as np

def check_orthogonal( name1, name2 ):
    data1 = np.loadtxt( name1 )
    data2 = np.loadtxt( name2 )
    print data1[0]*data2[0] + data1[1]*data2[1] + data1[2]*data2[2]
    print data1[3]*data2[3] + data1[4]*data2[4] + data1[5]*data2[5]
    print data1[6]*data2[6] + data1[7]*data2[7] + data1[8]*data2[8]

if __name__ == "__main__":
    print "Inner product k.E for 3 pulses in lab frame:"
    check_orthogonal( "res/kuvl.dat", "res/euvl.dat" )
    print "Inner product k.E for 3 pulses in mol frame:"
    check_orthogonal( "res/kuvm.dat", "res/euvm.dat" )
    
