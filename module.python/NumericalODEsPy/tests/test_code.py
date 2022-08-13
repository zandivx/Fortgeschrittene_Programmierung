y = 1.1051708333333332
f = lambda t, y: y
h = 1e-1
t = h


def v(r: float, f: float, k: float) -> float:
    return r + f * k


k1 = f(t, y)
k2 = f(t + h / 2, v(y, h / 2, k1))
k3 = f(t + h / 2, v(y, h / 2, k2))
k4 = f(t + h, v(y, h, k3))

y_1 = y + h / 6 * (k1 + 2 * (k2 + k3) + k4)

print(y_1)
