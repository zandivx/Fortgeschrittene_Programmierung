# type: ignore
import numpy as np
from matplotlib import pyplot as plt
from numericalodes import RK4c

TRIGGER = False


def trigger_PyArg_ParseTuple():
    RK4c(0)


def trigger_PySequence_Check():
    RK4c(0, 0, 0, 0, 0)


def trigger_PySequence_Length():
    RK4c([0, 0], 0, 0, [0], 0)


def trigger_PyCallable_Check():
    RK4c([0, 0], 0, 0, [0, 0], 0)


def trigger_PyNumber_Check():
    RK4c([np.sin], 0, 0, ["string"], 0)


def trigger_sanity_checks():
    RK4c([np.sin], 0, -1, [0], 1)
    RK4c([np.sin], 0, 1, [0], 0)


def trigger():
    for f in [
        trigger_PyArg_ParseTuple,
        trigger_PySequence_Check,
        trigger_PySequence_Length,
        trigger_PyCallable_Check,
        trigger_PyNumber_Check,
        trigger_sanity_checks,
    ]:
        try:
            f()
        except Exception as e:
            print(f"{f.__name__}:\t{e}\n")


def test_linear():
    rv = RK4c([lambda t, y: 4], 0, 1, [0], 1e-1)
    t = np.array(rv[0])
    y = np.array(rv[1][0])
    return t, y


def test_exp():
    rv = RK4c([lambda t, y: y[0]], 0, 5, [1], 1e-4)
    t = np.array(rv[0])
    y = np.array(rv[1][0])
    return t, y


def main() -> None:
    if TRIGGER:
        trigger()

    for f, analytic in zip([test_linear, test_exp], [lambda x: 4 * x, np.exp]):
        t, y = f()
        t_analytic = np.linspace(t[0], t[-1], 1000)
        plt.plot(t, y, label="C")
        plt.plot(t_analytic, analytic(t_analytic), label="analytic")
        plt.legend()
        plt.grid()
        plt.show()


if __name__ == "__main__":
    main()
