import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp
from numericalodes.dev.RungeKutta4Vector import RK4vec, phi_dot, omega_dot


def system(t, y):
    phi, omega = y
    return np.array([omega, -np.sin(phi)])


def main() -> None:
    df = pd.read_csv("output/game3.csv")
    t = np.arange(0, 4 * np.pi, 0.1)
    sol = solve_ivp(system, (0, 4 * np.pi), [np.pi * 0.9, 0])  # , rtol=1e-4)
    plt.plot(sol.t, sol.y[0], "--", label=f"solve_ivp")

    t_py, y_py = RK4vec([phi_dot, omega_dot], 2, 0, 4 * np.pi, [np.pi * 0.9, 0], 1e-4)
    plt.plot(t_py, y_py[:, 0], "-.", label="self_py")

    plt.plot(t, df["y0"], label="self_c")
    plt.legend()
    plt.savefig("output/game3.pdf")


if __name__ == "__main__":
    main()
