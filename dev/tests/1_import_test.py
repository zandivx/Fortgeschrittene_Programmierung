"""
Check if module is installed like intended
"""

import numericalodes as num
import numericalodes.c as c
import numericalodes.py as py

print(f"num:\n{dir(num)}")
print(f"c:\n{dir(c)}")
print(f"py:\n{dir(py)}")
