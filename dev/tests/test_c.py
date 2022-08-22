import numericalodes
import numpy as np
import matplotlib.pyplot as plt
from math import sin, pi


def main() -> None:
    t, y = numericalodes.RK4c([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 20, [0.1, 0], 1e-2)
    t = np.array(t)
    y = np.array(y)
    plt.plot(t, y[0])
    plt.show()


if __name__ == "__main__":
    main()
