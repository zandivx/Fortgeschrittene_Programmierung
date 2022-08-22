from setuptools import setup, Extension
from pathlib import Path

PATH_TO_C = Path("numericalodes/c")

# https://setuptools.pypa.io/en/latest/userguide/ext_modules.html
c = Extension(
    name="numericalodes.c",
    sources=[
        str(PATH_TO_C / s)
        for s in [
            "c.c",
            "RungeKutta4Py.c",
            "matrix.c",
            "vector.c",
        ]
    ],
)

with open("README.md") as f:
    long_desc = f.read()

# https://packaging.python.org/en/latest/guides/distributing-packages-using-setuptools/
# https://stackoverflow.com/questions/7948494/whats-the-difference-between-a-python-module-and-a-python-package

setup(
    name="numericalodes",
    version="1.0.dev31",
    description="Package with functions to calculate systems of ODEs numeracally similar to scipy.integrate.solve_ipv",
    long_description=long_desc,
    author="Andreas Zach",
    author_email="andreas.zach@student.tugraz.at",
    url="https://github.com/zandivx/numericalodes",
    license="MIT",
    # packages=["numericalodes"],
    ext_modules=[c],
)
