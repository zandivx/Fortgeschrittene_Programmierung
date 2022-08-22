from math import sin, pi
import matplotlib.pyplot as plt
import numericalodes


TUP1 = [lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 4 * pi, [0.1, 0], 1e-2
TUP2 = [lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 4 * pi, [0.9 * pi, 0], 1e-4


def main() -> None:
    t_c, y_c = numericalodes.RK4c(*TUP2)
    t_py, y_py = numericalodes.RK4py(*TUP2)

    plt.plot(t_c, y_c[0], label="C")
    plt.plot(t_py, y_py[0], label="Python")

    plt.grid()
    plt.legend()

    plt.show()


if __name__ == "__main__":
    main()
