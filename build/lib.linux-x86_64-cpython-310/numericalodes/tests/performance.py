from math import sin, pi
from timeit import default_timer as ts
from numericalodes import RK4c, RK4py

SOLVER = {"C": RK4c, "Py": RK4py}
TIMES = {"C": [], "Py": []}  # type: ignore
MEAN = {"C": 0, "Py": 0}


try:
    from scipy.integrate import solve_ivp

    SOLVER["Scipy"] = solve_ivp
    TIMES["Scipy"] = []
    MEAN["Scipy"] = 0
except ImportError:
    print("Package 'scipy' not installed, therfore no comparison with scipy possible!")


def main() -> None:
    tuples = (
        ([lambda t, y: y[0]], 0, 3, [1], 1e-6),
        ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [0, pi / 8], 1e-6),
        ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [pi * 0.9, 0], 1e-6),
        ([lambda t, y: y[0] / t + y[1] * 2 * t, lambda t, y: y[1] / t], 1, 5, [1, 1], 1e-6),
    )

    for tup in tuples:
        for key in SOLVER.keys():
            print(f"{key}:\t{tup}")
            for _ in range(1):
                start = ts()
                SOLVER[key](*tup)
                end = ts()
                TIMES[key].append(end - start)

    for key, times in TIMES.items():
        MEAN[key] = sum(times) / len(times)  # type: ignore

    print(f"Mean time over all test cases:\n{MEAN}")
    with open("performance.csv") as f:
        f.write(str(MEAN))


if __name__ == "__main__":
    main()
