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
        
        row = y[i-1]
        
        if i < 5:
            #print(f"{i=}\nt={t[i-1]}\n{row=}\n{k1=}\n{k2=}\n{k3=}\n{k4=}\n")
            pass
        
        # axis 1 loops
        
        # k1
        m = 0
        while m < n:
            k1[m] = func[m](t[i-1], *row)
            m += 1
            
        # k2
        m = 0
        while m < n:
            k2[m] = func[m](t[i-1] + h/2, *(row + h/2 * k1))
            m += 1
            
        # k3
        m = 0
        while m < n:            
            k3[m] = func[m](t[i-1] + h/2, *(row + h/2 * k2))
            m += 1
        
        if i < 5:
            print(f"{i=}\n{row=}\n{k2=}\ntmp={row + h/2 * k2}\n{k3=}\n")
            pass
            
        # k4
        m = 0
        while m < n:
            k4[m] = func[m](t[i-1] + h, *(row + h * k3))
            m += 1
        
        # next y
        m = 0
        while m < n:
            y[i,m] = y[i-1,m] + h/6 * (k1[m] + 2*(k2[m] + k3[m]) + k4[m])
            m += 1
        
        # next t  
        t[i] = t[i-1] + h
        
        i += 1
    
    print(f"{size=}")
    return t, y

def phi_dot(t, phi, omega):
    return omega

def omega_dot(t, phi, omega):
    return -math.sin(phi)

if __name__ == "__main__":
    t2, y2 = RK4vec([phi_dot, omega_dot], 2, 0, 4*np.pi, [np.pi*0.9, 0], 0.1)

    plt.plot(t2, y2[:,0])
    plt.savefig("output/python_plot.pdf")