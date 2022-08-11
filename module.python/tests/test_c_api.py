# type: ignore
import numpy as np
from matplotlib import pyplot as plt
import numericalodes

TRIGGER = False


def trigger_PyArg_ParseTuple():
    numericalodes.RK4(0)


def trigger_PySequence_Check():
    numericalodes.RK4(0, 0, 0, 0, 0)


def trigger_PySequence_Length():
    numericalodes.RK4([0, 0], 0, 0, [0], 0)


def trigger_PyCallable_Check():
    numericalodes.RK4([0, 0], 0, 0, [0, 0], 0)


def trigger_PyNumber_Check():
    numericalodes.RK4([np.sin], 0, 0, ["string"], 0)


def trigger_sanity_checks():
    numericalodes.RK4([np.sin], 0, -1, [0], 1)
    numericalodes.RK4([np.sin], 0, 1, [0], 0)


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
    rv = numericalodes.RK4([lambda t, y: 4], 0, 1, [0], 1e-1)
    t = np.array(rv[0])
    y = np.array(rv[1])
    return t, y


def test_exp():
    rv = numericalodes.RK4([lambda t, y: y], 0, 5, [1], 1e-4)
    t = np.array(rv[0])
    y = np.array(rv[1])
    return t, y


def main() -> None:
    if TRIGGER:
        trigger()

    for f, analytic in zip([test_linear, test_exp], [lambda x: 4 * x, np.exp]):
        t, y = f()
        t_analytic = np.linspace(t[0], t[-1], 1000)
        plt.plot(t, y.T, label="C")
        plt.plot(t_analytic, analytic(t_analytic), label="analytic")
        plt.legend()
        plt.grid()
        plt.show()


if __name__ == "__main__":
    main()
