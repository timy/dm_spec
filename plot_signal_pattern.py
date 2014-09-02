import matplotlib.pyplot as plt
import numpy as np
import pymod_data as pmd

data = np.loadtxt("res/kuvl.dat")
kv = [ data[0:3], data[3:6], data[6:9] ]

lv = pmd.var_direction()
#print l[0]*np.array([1,2,3])

#print np.array([1, 0]) * np.array([[1, 2], [2, 3]])

#print kv

for l in lv:
    s = np.array([0., 0., 0.])
    for ip in range(3):
        s += l[ip] * kv[ip]
    print s

#print map( lambda l: l*np.array([1,2,3]), lv[0] )

# k = k1 + k2 + k3
# k = k / np.abs(k[2])
# print k
