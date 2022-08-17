from math import sin, pi
from timeit import default_timer as ts
from numericalodes import RK4c, RK4py
from statistics import mean
import csv

SCIPY = False
REPETITIONS = 20
IVPS = (
    ([lambda t, y: y[0]], 0, 3, [1], 1e-5),
    ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [0, pi / 8], 1e-5),
    ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [pi * 0.9, 0], 1e-5),
    ([lambda t, y: y[0] / t + y[1] * 2 * t, lambda t, y: y[1] / t], 1, 5, [1, 1], 1e-5),
)


try:
    from scipy.integrate import solve_ivp

    def test_scipy() -> dict[str, float]:
        times = []

        for tup in IVPS:
            for _ in range(REPETITIONS):
                start = ts()
                funcs, t0, tmax, y0, h = tup
                system = lambda t, y: [func(t, y) for func in funcs]
                solve_ivp(system, [t0, tmax], y0, rtol=h)
                end = ts()
                times.append(end - start)

        return {"Scipy": mean(times)}

    SCIPY = True

except ImportError:
    print("Package 'scipy' not installed, therfore no comparison with scipy possible!")


def test_numericalodes() -> dict[str, float]:
    # solver = {"Py": RK4py}
    # times = {"Py": []}  # type: ignore
    solver = {"Py": RK4py, "C": RK4c}
    times = {"Py": [], "C": []}  # type: ignore

    for tup in IVPS:
        for key in solver.keys():
            for _ in range(REPETITIONS):
                start = ts()
                solver[key](*tup)  # type: ignore
                end = ts()
                times[key].append(end - start)

    return {key: mean(val) for key, val in times.items()}  # type: ignore


def main() -> None:
    results = test_numericalodes()

    if SCIPY:
        results.update(test_scipy())

    print(f"Mean time over all test cases:\n{results}")
    with open("performance.csv", "w") as f:
        writer = csv.DictWriter(f, results.keys())
        writer.writeheader()
        writer.writerow(results)


if __name__ == "__main__":
    main()
