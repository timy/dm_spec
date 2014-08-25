import matplotlib.pyplot as plt
import numpy as np

n_t = 2200

#data = np.loadtxt("res/ppar_2d_ 3_2.dat")

#data = np.loadtxt("res/ppar_test.dat")
#data = np.loadtxt("res/spec_test.dat")
#w = data[:,0]
#spec = data[:,1]**2 + data[:,2]**2

data = np.loadtxt("res/spec_2d_ 3_2.dat")

w = data[::n_t, 0]
r_sp = data[::n_t, 2]
i_sp = data[::n_t, 3]
sp = r_sp**2 + i_sp**2

plt.plot( w, sp, marker='.' )
plt.show()

#w_y, w_x = data[::n_x, 0], data[:n_x, 1]
# print w_x[699], w_x[700], w_x[701]
# dat = data[700:1201, 2] + data[700:1201, 3]*1j

# sp = np.fft.fft(dat)
# freq = np.fft.fftfreq(501) * 1.0 / 1.88492e-4
# plt.plot( freq, sp.real**2 + sp.imag**2 )
# plt.show()



#dat = data[700:, 3]


#print w_x

#spec = data[:n_x, 2]**2 + data[:n_x, 3]**2
#plt.plot(spec)
#plt.show()
