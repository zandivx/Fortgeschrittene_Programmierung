import numpy as np
import numericalodes
import matplotlib.pyplot as plt

rv = numericalodes.RK4([np.sin], 0, 0, [1], 1e-4)
print(rv)
