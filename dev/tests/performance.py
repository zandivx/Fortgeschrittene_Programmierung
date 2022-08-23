"""
Performance test of numericalodes: Comparison of C code, Python code and -- if available -- scipy code.
Call the script via command line and pass a path as first argument to write the performance output to a file.
"""

from math import pi, sin
from statistics import mean
from sys import argv
from timeit import default_timer as ts
from typing import Callable, Sequence
from numericalodes import RK4c, RK4py  # type: ignore

VERBOSE = True


def performance_test(solver: Callable, ivps: Sequence[tuple], rep: int = 20, scipy: bool = False) -> list[float]:
    mean_times = []

    if scipy:
        for tup in ivps:
            total_time = 0.0
            for i in range(rep):
                funcs, t0, tmax, y0, h = tup
                system = lambda t, y: [func(t, y) for func in funcs]
                start = ts()
                solver(system, [t0, tmax], y0, method="RK45", first_step=h, max_step=h)
                end = ts()
                total_time += end - start
                if VERBOSE:
                    print(f"{solver.__name__}: {tup}\t@ rep {i}")
            mean_times.append(total_time / rep)

    else:
        for tup in ivps:
            total_time = 0.0
            for i in range(rep):
                start = ts()
                solver(*tup)
                end = ts()
                total_time += end - start
                if VERBOSE:
                    print(f"{solver.__name__}: {tup}\t@ rep {i}")
            mean_times.append(total_time / rep)

    return mean_times


def main() -> None:

    STEP_SIZE = 1e-4

    IVPS = (
        ([lambda t, y: y[0]], 0, 3, [1], STEP_SIZE),
        ([lambda t, y: -15 * y[0]], 0, 1e3, [1], 1e-1),  # stiff problem: https://en.wikipedia.org/wiki/Stiff_equation
        ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [0, pi / 8], STEP_SIZE),
        ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [pi * 0.9, 0], STEP_SIZE),
        ([lambda t, y: y[0] / t + y[1] * 2 * t, lambda t, y: y[1] / t], 1, 5, [1, 1], STEP_SIZE),
    )

    means = {}

    means["C"] = performance_test(RK4c, IVPS)
    means["Python"] = performance_test(RK4py, IVPS)

    try:
        from scipy.integrate import solve_ivp

        means["Scipy"] = performance_test(solve_ivp, IVPS, scipy=True)

    except ImportError:
        print("Scipy not installed, so no comparison possible")

    total_means = {key: mean(value) for key, value in means.items()}

    print(f"Mean times: {means}")
    print(f"Total means: {total_means}")

    if len(argv) > 1:
        path = argv[1]

        try:
            import pandas as pd

            df1 = pd.DataFrame(means)
            df2 = pd.DataFrame(total_means, index=["total mean"])
            df = pd.concat([df1, df2])
            df.to_csv(path)

        except ImportError:
            from csv import DictWriter

            with open(path, "w") as f:
                writer = DictWriter(f, means.keys())
                writer.writeheader()
                writer.writerow(means)
                writer.writerow(total_means)


if __name__ == "__main__":
    main()
