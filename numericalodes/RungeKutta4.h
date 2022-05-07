#ifndef RUNGE_KUTTA_4_H
#define RUNGE_KUTTA_4_H

double **RK4single(double (*func)(double, double), double t0, double tmax, double y0, double h);
double **RK4vector(double (**func)(double, double), int n, double t0, double tmax, double y0, double h);

#endif