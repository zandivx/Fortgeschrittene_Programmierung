#include "src/RungeKutta4.h"
#include "src/matrix.h"
#include "src/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793115997963468544185161590576171875
#define UNUSED(x) (void)(x)

double f1(double t, double y);
double f2(double t, double y);
double f2tilde(double t, double *y);
double phi_dot(double t, double *y);
double omega_dot(double t, double *y);

int game1();
int game2();
int game3();
int game4();
int testmatrix();
int testvector();
int test_v_add_v_factor_tmp();

//----------------------------------------------------------------------------------------------
double *Y = NULL;
int main()
{
    return game3();
}
//----------------------------------------------------------------------------------------------

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
    double *t, (*funcs[2])(double, double *), y0[2];

    funcs[0] = phi_dot;
    funcs[1] = omega_dot;
    y0[0] = PI * 0.9;
    y0[1] = 0;

    size = RK4vector(&t, &Y, funcs, 2, 0, 4 * PI, y0, 0.1);
    matrix m = {Y, size, 2};

    // print_m(m);
    fprint_m(m, "output/game3.csv");
    printf("size: %ld\n", size);

    free(t);
    free(Y);

    return 0;
}

int game4()
{
    size_t size = 0;
    const size_t n = 1;
    double *t, (*funcs[n])(double, double *), y0[n];

    funcs[0] = f2tilde;
    y0[0] = 1;

    size = RK4vector(&t, &Y, funcs, n, 0, 10, y0, 1e-4);
    matrix m = {Y, size, n};

    // print_m(m);
    fprint_m(m, "output/game3.csv");
    printf("size: %ld\n", size);

    free(t);
    free(Y);

    return 0;
}

int testmatrix()
{
    matrix m = {NULL, 5, 8};
    vector v = {NULL, m.c};
    create_m(&m);

    for (size_t i = 0; i < m.r; i++)
    {
        for (size_t j = 0; j < m.c; j++)
        {
            set(m, i, j, i + j);
        }
    }
    print_m(m);

    v.ptr = get_r(m, 3);
    print_v(v);

    print_m(m);

    return 0;
}

int testvector()
{
    vector v1 = {.n = 6};
    vector v2 = {.n = 6};
    vector v3 = {.n = 6};

    create_v(&v1);
    create_v(&v2);
    create_v(&v3);

    for (size_t i = 0; i < v1.n; i++)
    {
        v1.ptr[i] = 2;
    }
    for (size_t i = 0; i < v2.n; i++)
    {
        v2.ptr[i] = i;
    }

    v_add_v_factor_tmp(v3, v1, v2, 3);
    print_v(v3);

    return 0;
}

// works as intended
int test_v_add_v_factor_tmp()
{
    double h = 0.1;
    double row_arr[2] = {2.82123344, -0.06219571};
    double k2_arr[2] = {-0.07794109, -0.31785755};
    vector row = {row_arr, 2};
    vector k2 = {k2_arr, 2};
    vector tmp = {NULL, 2};

    create_v(&tmp);
    v_add_v_factor_tmp(tmp, row, k2, h / 2);
    print_v(row);
    print_v(k2);
    print_v(tmp);

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

double f2tilde(double t, double *y)
{
    UNUSED(t);
    return y[0];
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
