from setuptools import setup, Extension

setup(
    ext_modules=[
        Extension(name="numericalodes", sources=["numericalodes/numericalodes.c"])
    ]
)
