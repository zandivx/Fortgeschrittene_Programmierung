"""
Plot the curve of solutions to some selected IVP's and their analytical solution as well.
"""

from math import pi, sin, sqrt
import os
from typing import Callable
import numpy as np
import matplotlib.pyplot as plt
import numericalodes

plt.style.use("seaborn-dark")


def linear(t: np.ndarray) -> np.ndarray:
    return t


def exp_half_squared(t: np.ndarray) -> np.ndarray:
    return np.exp(0.5 * t**2)


def exp_cos(t: np.ndarray) -> np.ndarray:
    return np.e * np.exp(-np.cos(t)) - 1


def main() -> None:
    STEP_SIZE = 1e-4

    numerical = (
        ([lambda t, y: y[0]], 0, 2.5, [1], STEP_SIZE),
        ([lambda t, y: y[1], lambda t, y: -y[0]], 0, 2 * pi, [0, 1], STEP_SIZE),
        ([lambda t, y: 1], 0, 6, [0], STEP_SIZE),
        ([lambda t, y: t * y[0]], 0, 2.2, [1], STEP_SIZE),
        ([lambda t, y: sin(t) * (1 + y[0])], 0, 2 * pi, [0], STEP_SIZE),
        ([lambda t, y: 1 / (2 * sqrt(t))], 0.01, 6, [0.1], STEP_SIZE),
    )
    analytical: tuple[Callable] = (np.exp, np.sin, linear, exp_half_squared, exp_cos, np.sqrt)  # type: ignore

    _, ax = plt.subplots(figsize=(12, 7))

    for num, an in zip(numerical, analytical, strict=True):
        name = an.__name__
        t, y = numericalodes.RK4c(*num)
        ax.plot(t, y[0], linewidth=2.5, label=f"{name}: numerically")
        ax.plot(t, an(np.array(t)), linestyle="--", linewidth=1.5, color="w", label=f"{name}: analytically")

    ax.set_title("Compare numerical to analytical solutions")
    ax.grid()
    ax.legend(loc="upper right")
    plt.tight_layout()

    directory = "output"
    if not os.path.isdir(directory):
        os.makedirs(directory)

    plt.savefig(f"{directory}/compare_analytic.pdf")
    plt.show()


if __name__ == "__main__":
    main()
