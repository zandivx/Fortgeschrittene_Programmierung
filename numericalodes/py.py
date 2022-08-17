from math import ceil
from itertools import repeat
from typing import Callable, Sequence


def RK4py(
    funcs: Sequence[Callable], t0: float, tmax: float, y0: Sequence[float], h: float
) -> tuple[list[float], list[list[float]]]:
    # compare: DOC_numericalodesc
    """Calculate the solution to the explicit system of differential equations y'=f(t,y) (with y and f as vectors) numerically
    using the explicit Runge-Kutta algorithm of order 4 written in pure Python.
    'Sequence' is a Python object that supports the sequence protocol (e.g. a list or a tuple)
    funcs: sequence\tentries of the vector f(t,y) (Callables)
    t0: float\tleft border of the domain to calculate the solution on
    tmax: float\tright border of the domain to calculate the solution on
    y0: sequence\tentries of the vector with initial conditions at the moment t0
    h: float\tstep size (distance between moments t)
    """

    n = len(funcs)
    size = ceil((tmax - t0) / h)

    # initialize vectors and matrix
    k1 = [0.0] * n
    k2 = [0.0] * n
    k3 = [0.0] * n
    k4 = [0.0] * n
    t = [0.0] * size

    #! pointer bug in C: y = [[0.0] * n] * size
    # changing one value in one sub-list changes the value in every sub-list
    y = [[0.0] * n for _ in range(size)]  # type: ignore
    # size x n-->
    #  |
    #  V

    # insert initial values
    t[0] = t0
    t[size - 1] = tmax
    y[0] = list(y0)

    def f_add_f_factor(r: float, f: float, k: float) -> float:
        return r + f * k

    for i in range(1, size):

        cur_row = y[i - 1]
        cur_t = t[i - 1]

        for j in range(n):
            k1[j] = funcs[j](cur_t, *cur_row)

        for j in range(n):
            k2[j] = funcs[j](cur_t + h / 2, *map(f_add_f_factor, cur_row, repeat(h / 2), k1))

        for j in range(n):
            k3[j] = funcs[j](cur_t + h / 2, *map(f_add_f_factor, cur_row, repeat(h / 2), k2))

        for j in range(n):
            k4[j] = funcs[j](cur_t + h, *map(f_add_f_factor, cur_row, repeat(h), k3))

        for j in range(n):
            y[i][j] = y[i - 1][j] + h / 6 * (k1[j] + 2 * (k2[j] + k3[j]) + k4[j])

        t[i] = cur_t + h

    # transpose y matrix
    # https://stackoverflow.com/a/6473724/16527499
    y = list(map(list, zip(*y)))

    return t, y
