"""
Plot the curve of the solution of several ivp's to demonstrate the package numericalodes.
"""

from numpy import pi, sin
import matplotlib.pyplot as plt
from numericalodes import RK4c, RK4py  # type: ignore


def main() -> None:
    STEP_SIZE = 1e-5
    IVPS = (
        ([lambda t, y: y[0]], 0, 3, [1], STEP_SIZE),
        ([lambda t, y: -15 * y[0]], 0, 1, [1], STEP_SIZE),  # https://en.wikipedia.org/wiki/Stiff_equation
        ([lambda t, y: y[1], lambda t, y: -y[0]], 0, 2 * pi, [0, 1], STEP_SIZE),
        ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 8 * pi, [pi * 0.99, 0], STEP_SIZE),
        ([lambda t, y: y[0] / t + y[1] * 2 * t, lambda t, y: y[1] / t], 0.1, 5, [-4, 1], STEP_SIZE),
    )

    for i, tup in enumerate(IVPS):

        _, ax = plt.subplots()

        for name, solver in zip(["Py", "C"], [RK4py, RK4c]):
            t, y = solver(*tup)
            fmt = "-" if name == "Py" else "--"
            ax.plot(t, y[0], fmt, label=f"{name}: IVP{i}")

        ax.legend()
        ax.grid()
        plt.tight_layout()
        plt.savefig(f"dev/output/IVP{i}.pdf")

        print(f"IVP{i} done")


if __name__ == "__main__":
    main()
