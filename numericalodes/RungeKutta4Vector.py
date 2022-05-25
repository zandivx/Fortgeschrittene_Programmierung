import numpy as np
import math
import matplotlib.pyplot as plt



def RK4vec(func: list, n: int, t0: float, tmax: float, y0: list, h: float):
    size = math.ceil((tmax - t0) / h)
    # print(f"{size=}")
    
    # vectors and matrix
    k1 = np.empty(n)
    k1 = np.empty(n)
    k2 = np.empty(n)
    k3 = np.empty(n)
    k4 = np.empty(n)
    t = np.empty(size)
    y = np.empty((size, n))
    
    # initialize arrays
    t[0] = t0
    t[size-1] = tmax
    y[0] = y0
        
    # axis 0
    i = 1
    while i < size:
        # print(f"{i=}")
        
        row = y[i-1]
        
        # axis 1 loops
        m = 0
        while m < n:
            # print(f"k1: {i=}, {m=}")
            k1[m] = func[m](t[i-1], *(row))
            m += 1
            
        m = 0
        while m < n:
            # print(f"k2: {i=}, {m=}")
            k2[m] = func[m](t[i-1] + h/2, *(row + h/2 * k1))
            m += 1
            
        m = 0
        while m < n:
            # print(f"k3: {i=}, {m=}")
            k3[m] = func[m](t[i-1] + h/2, *(row + h/2 * k2))
            m += 1
            
        m = 0
        while m < n:
            # print(f"k4: {i=}, {m=}")
            k4[m] = func[m](t[i-1] + h, *(row + h * k3))
            m += 1
            
        m = 0
        while m < n:
            # print(f"m: {i=}, {m=}")
            y[i,m] = y[i-1,m] + h/6 * (k1[m] + 2*(k2[m] + k3[m]) + k4[m])
            m += 1
            
        t[i] = t[i-1] + h
        
        i += 1
        
    return t, y

def phi_dot(t, phi, omega):
    return omega

def omega_dot(t, phi, omega):
    return -math.sin(phi)

if "__name__" == "__main__":
    # t1, y1 = RK4vec([lambda t, y: y], 1, 0, 10, [1], 0.1)
    t2, y2 = RK4vec([phi_dot, omega_dot], 2, 0, 10, [math.pi/8, 0], 0.1)

    print(t2)
    print(y2)

    plt.plot(t2, y2[:,0])
    plt.savefig("output/python_plot.pdf")