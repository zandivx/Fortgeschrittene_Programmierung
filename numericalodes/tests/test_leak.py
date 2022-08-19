from numericalodes import RK4c
from math import pi, sin

LOC = 0

IVPS = (
    ([lambda t, y: -15 * y[0]], 0, 1e3, [1], 1e-1),  # stiff problem: https://en.wikipedia.org/wiki/Stiff_equation
    ([lambda t, y: y[0]], 0, 3, [1], 1e-5),
    ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [0, pi / 8], 1e-5),
    ([lambda t, y: y[1], lambda t, y: -sin(y[0])], 0, 2 * pi, [pi * 0.9, 0], 1e-5),
    ([lambda t, y: y[0] / t + y[1] * 2 * t, lambda t, y: y[1] / t], 1, 5, [1, 1], 1e-5),
)


def main() -> None:
    for i in range(100):
        RK4c(*IVPS[LOC])
        print(f"Passed {i}")


if __name__ == "__main__":
    main()
