import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

df = pd.read_csv("output/game3.csv")
t = np.arange(0, 4*np.pi, 0.01)

def system(t, y):
    phi, omega = y
    return np.array([omega, -np.sin(phi)])


sol = solve_ivp(system, (0, 4*np.pi), [np.pi/8, 0], rtol=1e-10)
plt.plot(sol.t, sol.y[0], "--", label=f"solve_ivp")


plt.plot(t, df["y0"], label="self")
plt.legend()
plt.savefig("output/array2.pdf")