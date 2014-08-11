import matplotlib.pyplot as plt
import numpy as np

n_y = 600
n_x = 1400

i_start = 500

data = np.loadtxt("res/ppar_2d_ 0_2.dat")


x_min, x_max = 0.0, 2.0
y_min, y_max = 0.0, 1.0

y = data[::n_x, 0]
x = data[:n_x, 1]


# cut along y-axis
# i_start, = np.nonzero( (x >= x_min) & (x <= x_max) )
# i_start = i_start[0]
# r_sp = data[i_start::n_x, 2]
# i_sp = data[i_start::n_x, 3]
# sp = r_sp**2 + i_sp**2
# w = y

# cut along x-axis
i_start, = np.nonzero( (y >= y_min) & (y <= y_max) )
i_start = i_start[0]
print("i_start = ",  i_start)
print("y = ", y[i_start])
print("y = ", data[i_start*n_x, 0])

r_sp = data[i_start*n_x:(i_start+1)*n_x, 2]
i_sp = data[i_start*n_x:(i_start+1)*n_x, 3]
#sp = r_sp**2 + i_sp**2
sp = r_sp
w = x

plt.plot( w, sp, marker='.' )
plt.show()
