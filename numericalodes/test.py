import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("array.csv")

t = np.linspace(0, 12, 1000)

y2 = -np.cos(t)

plt.plot(df["t"], df["y"], label="numerically")
plt.plot(t, y2, "--", label="exact")
plt.legend()
plt.savefig("array.pdf")