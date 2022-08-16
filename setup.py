from setuptools import setup, Extension

PATH_TO_C = "numericalodes/c"

# https://setuptools.pypa.io/en/latest/userguide/ext_modules.html
numericalodesc = Extension(
    name="numericalodes.c",
    sources=[
        f"{PATH_TO_C}/{s}"
        for s in [
            "numericalodesc.c",
            "RungeKutta4Py.c",
            "matrix.c",
            "vector.c",
        ]
    ],
)

setup(
    name="numericalodes",
    version="0.1",
    description="C Extension Module to calculate systems of ODEs numeracally similar to scipy.integrate.solve_ipv."
    "This is a voluntary project for the Technical University of Graz",
    author="Andreas Zach",
    author_email="andreas.zach@student.tugraz.at",
    url="https://github.com/zandivx/numericalodes",
    license="MIT License",
    packages=["numericalodes.c", "numericalodes.py"],
    ext_modules=[numericalodesc],
)
