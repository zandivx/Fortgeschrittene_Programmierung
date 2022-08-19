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

setup(
    name="numericalodes",
    # https://packaging.python.org/en/latest/guides/distributing-packages-using-setuptools/
    version="1.0.dev27",
    # compare: DOC_numericalodesc
    description="Package with functions to calculate systems of ODEs numeracally similar to scipy.integrate.solve_ipv",
    long_description=long_desc,
    author="Andreas Zach",
    author_email="andreas.zach@student.tugraz.at",
    url="https://github.com/zandivx/numericalodes",
    license="MIT",
    # https://stackoverflow.com/questions/7948494/whats-the-difference-between-a-python-module-and-a-python-package
    packages=["numericalodes", "numericalodes.tests"],
    ext_modules=[c],
)
