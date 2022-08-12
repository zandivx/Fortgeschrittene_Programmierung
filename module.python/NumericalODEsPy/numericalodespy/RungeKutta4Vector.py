from math import ceil
from itertools import repeat
from typing import Callable


def RK4(
    func: list[Callable], t0: float, tmax: float, y0: list[float], h: float
) -> tuple[list[float], list[list[float]]]:

    n = len(func)
    size = ceil((tmax - t0) / h)

    # initialize vectors and matrix
    k1 = [0.0] * n
    k1 = [0.0] * n
    k2 = [0.0] * n
    k3 = [0.0] * n
    k4 = [0.0] * n
    t = [0.0] * size

    # size x n matrix
    row = [0.0] * n
    y = [row] * size

    # put initial conditions in arrays
    t[0] = t0
    t[size - 1] = tmax
    y[0] = y0

    def f_add_f_factor(r: float, f: float, k: float) -> float:
        return r + f * k

    # axis 0
    i = 1
    while i < size:

        # axis 1 loops
        row = y[i - 1]

        # k1
        j = 0
        while j < n:
            k1[j] = func[j](t[i - 1], *row)
            j += 1

        # k2
        j = 0
        while j < n:
            k2[j] = func[j](t[i - 1] + h / 2, *map(f_add_f_factor, row, repeat(h / 2), k1))
            j += 1

        # k3
        j = 0
        while j < n:
            k3[j] = func[j](t[i - 1] + h / 2, *map(f_add_f_factor, row, repeat(h / 2), k2))
            j += 1

        # k4
        j = 0
        while j < n:
            k4[j] = func[j](t[i - 1] + h, *map(f_add_f_factor, row, repeat(h), k3))
            j += 1

        # next y
        j = 0
        while j < n:
            y[i][j] = y[i - 1][j] + h / 6 * (k1[j] + 2 * (k2[j] + k3[j]) + k4[j])
            j += 1

        # next t
        t[i] = t[i - 1] + h

        i += 1

    # transpose y matrix
    # https://stackoverflow.com/a/6473724/16527499
    y = list(map(list, zip(*y)))

    return t, y
