import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

df = pd.read_csv("output/game3.csv")
t = np.arange(0, 16*np.pi, 1e-4)

def system(t, y):
    phi, omega = y
    return np.array([omega, -np.sin(phi)])


sol = solve_ivp(system, (0, 16*np.pi), [np.pi*0.9, 0], rtol=1e-4)
plt.plot(sol.t, sol.y[0], "--", label=f"solve_ivp")


plt.plot(t, df["y0"], label="self")
plt.legend()
plt.savefig("output/game3.pdf")