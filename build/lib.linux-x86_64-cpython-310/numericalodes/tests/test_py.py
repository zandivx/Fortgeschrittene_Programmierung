import numericalodes
import numpy as np
import matplotlib.pyplot as plt
from timeit import default_timer as ts


def main() -> None:
    tup1 = ([lambda t, y: y[0]], 0, 3, [1], 1e-4)
    tup1_alt = ([lambda t, y: y], 0, 3, [1], 1e-4)
    tup2 = ([lambda t, y: y[1], lambda t, y: -np.sin(y[0])], 0, 2 * np.pi, [0, np.pi / 8], 1e-6)

    start = ts()
    # t, ypy = numericalodes.RK4py(*tup2)  # type: ignore
    py = ts()
    t, yc = numericalodes.RK4c(*tup1)
    c = ts()

    print(f"Python: {py-start} s\nC: {c-py} s")

    print(np.array(yc))
    # print(np.array(ypy))

    # plt.plot(t, ypy[0], label="Python")
    plt.plot(t, yc[0], label="C")
    plt.plot(t, np.exp(t), label="analytic")

    plt.grid()
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()
