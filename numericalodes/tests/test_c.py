import numericalodes
import numpy as np
import matplotlib.pyplot as plt
from math import sin, pi


def main() -> None:
    t, y = numericalodes.RK4c([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [pi / 8, 0], 1e-1)
    y = np.array(y)
    t = np.array(t)
    print(y[0])
    print(t)
    plt.plot(t, y[0])
    plt.show()


if __name__ == "__main__":
    main()
