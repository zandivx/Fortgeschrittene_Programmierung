import numericalodespy
import numericalodesc
import matplotlib.pyplot as plt
from numpy import array


def main() -> None:
    rv = numericalodespy.RK4([lambda t, y: y], 0, 3, [1], 1e-2)
    print(rv)


if __name__ == "__main__":
    main()
