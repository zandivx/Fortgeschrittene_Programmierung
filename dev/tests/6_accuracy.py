"""
Check the accuracy of the C, Python and SciPy code to the analytical solution (sin(t)) of the
Taylor-approximated pendulum equation of motion: d^2y/dt^2 + y = 0
"""

import os
import numpy as np
from scipy.integrate import solve_ivp
from numericalodes import RK4c, RK4py  # type: ignore


def main() -> None:
    funcs = [lambda t, y: y[1], lambda t, y: -y[0]]
    t0 = 0
    tmax = 2 * np.pi
    y0 = [0, 1]
    h = 1e-5
    system = lambda t, y: [func(t, y) for func in funcs]

    t, c = RK4c(funcs, t0, tmax, y0, h)
    print("C done")
    _, py = RK4py(funcs, t0, tmax, y0, h)
    print("Py done")
    sol = solve_ivp(system, [t0, tmax], y0, method="RK45", first_step=h, max_step=h)
    print("Scipy done")

    y = np.sin(t)
    c_squares = np.sum((y - c[0]) ** 2)
    py_squares = np.sum((y - py[0]) ** 2)
    scipy_squares = np.sum((np.sin(sol.t) - sol.y[0]) ** 2)

    string = f"Least squares:\n{c_squares=}\n{py_squares=}\n{scipy_squares=}"
    print(string)

    directory = "output"
    if not os.path.isdir(directory):
        os.makedirs(directory)

    with open(f"{directory}/accuracy.txt", "w") as f:
        f.write(string)


if __name__ == "__main__":
    main()
