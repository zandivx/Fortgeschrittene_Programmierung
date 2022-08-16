# numericalodes
Python module written in C as project for SE "Fortgeschrittene Programmierung in der Physik" (PHY.W14UF) at the Technical University of Graz in Summer 2022.
Student: Andreas Zach
Professor: Enrico Arrigoni

## Aim
The aim is to implement known algorithms in order to solve ODEs and systems of ODEs numerically. Those algorithms are written in C and also in Python to compute performance differences statistically.
The module is written in C to get to know the Python-C-API and to practice coding in C. The module is primarily focused on solving the pendulum equation of motion without taylor approximation, but it should work for arbitrary ODEs and systems of those.

## Installation
Clone the repository:
```bash
git clone git@github.com:zandivx/numericalodes.git
cd numericallyodes
```
and then pip-install it
```bash
pip install .
```
