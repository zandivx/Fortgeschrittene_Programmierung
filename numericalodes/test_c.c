#include "RungeKutta4.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double f(double t, double y);

int main()
{
    double size, **array;

    array = RK4single(f, 0, 4 * M_PI, 1, 0.01);
    size = **(array + 2);

    printf("t: ");
    for (int n = 0; n < size; n++)
    {
        printf("%f", *((*array) + n));
    }

    printf("y: ");
    for (int n = 0; n < size; n++)
    {
        printf("%f", *(*(array + 1) + n));
    }

    return 0;
}

double f(double t, double y)
{
    return sin(t);
}