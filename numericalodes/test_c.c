#include "RungeKutta4.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define UNUSED(x) (void)(x)

double f1(double t, double y);
double f2(double t, double y);
double phi_dot(double t, double *y);
double omega_dot(double t, double *y);

int game1();
int game2();
int game3();
int testmatrix();

//---------------------------------------------------
double *Y;
int main()
{
    return game3();
}
//---------------------------------------------------

int game1()
{
    size_t size;
    double *t, *y;
    FILE *file = fopen("output/game1.csv", "w");

    size = RK4single(&t, &y, f1, 0, 4 * PI, -1, 0.01);

    fprintf(file, "t,y\n");
    for (size_t n = 0; n < size; n++)
    {
        fprintf(file, "%f,%f\n", t[n], y[n]);
    }

    fclose(file);

    free(t);
    free(y);

    return 0;
}

int game2()
{
    size_t size;
    double *t, *y;
    FILE *file = fopen("output/game2.csv", "w");

    size = RK4single(&t, &y, f2, 0, 10, 1, 0.01);

    fprintf(file, "t,y\n");
    for (size_t n = 0; n < size; n++)
    {
        fprintf(file, "%f,%f\n", t[n], y[n]);
    }

    fclose(file);

    free(t);
    free(y);

    return 0;
}

int game3()
{
    size_t size = 0;
    const size_t n = 2;
    double *t, (*funcs[n])(double, double *), y0[n];

    funcs[0] = phi_dot;
    funcs[1] = omega_dot;
    y0[0] = PI / 8;
    y0[1] = 0;

    size = RK4vector(&t, &Y, funcs, n, 0, 4 * PI, y0, 0.01);
    matrix m = {&Y, size, n};

    // print_m(m);
    fprint_m(m, "output/game3.csv");
    printf("size: %ld\n", size);

    free(t);
    free(Y);

    return 0;
}

int testmatrix()
{
    double **y = NULL;
    matrix m = {y, 5, 8};
    create_m(m);
    print_m(m);

    return 0;
}

double f1(double t, double y)
{
    UNUSED(y);
    return sin(t);
}

double f2(double t, double y)
{
    UNUSED(t);
    return y;
}

double phi_dot(double t, double *y)
{
    /*
    y[0] == phi
    y[1] == omega
    */
    UNUSED(t);
    return y[1];
}

double omega_dot(double t, double *y)
{
    UNUSED(t);
    return -sin(y[0]);
}
