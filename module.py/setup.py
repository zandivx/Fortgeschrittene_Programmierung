from setuptools import setup, Extension

PREFIX = "numericalodes"

setup(
    name="numericalodes",
    version="0.1",
    description="C Extension Module to calculate systems of ODEs numeracally similar to scipy.integrate.solve_ipv."
    "This is a learning project for the Technical University of Graz",
    author="Andreas Zach",
    author_email="andreas.zach@student.tugraz.at",
    url="https://github.com/zandivx/numericalodes",
    license="MIT License",
    include_dirs=["numericalodes"],
    ext_modules=[
        Extension(
            name="numericalodes",
            sources=[
                f"{PREFIX}/{s}"
                for s in [
                    "numericalodes.c",
                    "RungeKutta4Py.c",
                    "matrix.c",
                    "vector.c",
                ]
            ],
        )
    ],
)
