# type: ignore
import numpy as np
import numericalodes


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


def main() -> None:
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


if __name__ == "__main__":
    main()
