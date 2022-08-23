"""
Calculate the numerical solution to the pendulum equation of motion without Taylor-approximation
d^2y/dt^2 + sin(y) = 0
with varying initial conditions
"""

from math import sin, pi
import matplotlib.pyplot as plt
import numericalodes


def main() -> None:
    y0_tup = [0.1, 0], [0.99 * pi, 0], [0, 0.1 * pi], [0, 0.635 * pi], [0, 0.64 * pi]
    tup = [lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 6 * pi

    _, ax = plt.subplots()

    for y0 in y0_tup:
        t, y = numericalodes.RK4c(*tup, y0, 1e-4)
        ax.plot(t, y[0], label=str(y0))

    ax.grid()
    ax.legend()
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
