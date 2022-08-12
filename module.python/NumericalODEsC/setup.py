from setuptools import setup, Extension

PREFIX = "numericalodesc"

setup(
    name="numericalodesc",
    version="0.1",
    description="C Extension Module to calculate systems of ODEs numeracally similar to scipy.integrate.solve_ipv."
    "This is a voluntary project for the Technical University of Graz",
    author="Andreas Zach",
    author_email="andreas.zach@student.tugraz.at",
    url="https://github.com/zandivx/numericalodes",
    license="MIT License",
    ext_modules=[
        Extension(
            name="numericalodesc",
            sources=[
                f"{PREFIX}/{s}"
                for s in [
                    "numericalodesc.c",
                    "RungeKutta4Py.c",
                    "matrix.c",
                    "vector.c",
                ]
            ],
        )
    ],
)
