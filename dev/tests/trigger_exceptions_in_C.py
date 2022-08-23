"""
Trigger exceptions in C code
"""

from math import sin
from numericalodes import RK4c  # type: ignore


def trigger_PyArg_ParseTuple() -> None:
    RK4c(0)


def trigger_PySequence_Check() -> None:
    RK4c(0, 0, 0, 0, 0)


def trigger_PySequence_Length() -> None:
    RK4c([0, 0], 0, 0, [0], 0)


def trigger_PyCallable_Check() -> None:
    RK4c([0, 0], 0, 0, [0, 0], 0)


def trigger_PyNumber_Check() -> None:
    RK4c([sin], 0, 0, ["string"], 0)


def trigger_sanity_checks() -> None:
    RK4c([sin], 0, -1, [0], 1)
    RK4c([sin], 0, 1, [0], 0)


def trigger() -> None:
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


def main() -> None:
    trigger()


if __name__ == "__main__":
    main()
