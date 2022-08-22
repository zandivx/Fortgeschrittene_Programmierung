import numpy as np
import matplotlib.pyplot as plt
import numericalodes


def main() -> None:
    tup = ([lambda t, y: y[0]], 0, 3, [1], 1e-4)
    t, ypy = numericalodes.RK4py(*tup)
    _, yc = numericalodes.RK4c(*tup)

    print(np.array(ypy))
    print(np.array(yc))

    plt.plot(t, ypy[0], label="Python")
    plt.plot(t, yc[0], label="C")
    plt.plot(t, np.exp(t), label="analytic")

    plt.grid()
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()
