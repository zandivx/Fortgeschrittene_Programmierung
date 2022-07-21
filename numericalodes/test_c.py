import numpy as np
import numericalodes

rv = numericalodes.RK4([np.sin, np.exp], 0, 0, [1, 1], 1e-4)
print(rv)
