import numpy as np
import math
import matplotlib.pyplot as plt



def RK4vec(func: list, n: int, t0: float, tmax: float, y0: list, h: float):
    size = math.ceil((tmax - t0) / h)
    
    # vectors
    t = np.empty(size)
    k1 = np.empty(n)
    k1 = np.empty(n)
    k2 = np.empty(n)
    k3 = np.empty(n)
    k4 = np.empty(n)
    
    # matrix
    y = np.empty((n, size))
    
    # initialize arrays
    t[0] = t0
    y[:,0] = y0
    
    
    m = 1  # axis 0
    i = 0  # axis 1
    while i < size:
        while m < n:
            k1[m] = func[m](t[i-1], *(y[:,i-1]))
            k2[m] = func[m](t[i-1] + h/2, *(y[:,i-1] + h * float(k1[m]) / 2))
            k3[m] = func[m](t[i-1] + h/2, *(y[:,i-1] + h * float(k2[m]) / 2))
            k4[m] = func[m](t[i-1] + h, *(y[:,i-1] + h * float(k3[m])))

            t[i] = t[i-1] + h
            y[m][i] = y[m][i-1] + h/6 * (k1[m] + 2 * (k2[m] + k3[m]) + k4[m])

            print(f"{m=}")
            m += 1
            
        print(f"{i=}")
        i += 1
        
    return t, y

def phi_dot(t, phi, omega):
    return omega

def omega_dot(t, phi, omega):
    return -math.sin(phi)


# t1, y1 = RK4vec([lambda t, y: y], 1, 0, 10, [1], 0.1)
t2, y2 = RK4vec([phi_dot, omega_dot], 2, 0, 10, [math.pi/8, 0], 0.1)

print(t2)
print(y2)

plt.plot(t2, y2[1])
plt.savefig("plot.pdf")