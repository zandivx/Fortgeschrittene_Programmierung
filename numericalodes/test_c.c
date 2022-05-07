#include "RungeKutta4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

double f(double t, double y);

int main()
{
    size_t size;
    double *t, *y;
    FILE *file = fopen("array.csv", "w");

    size = RK4single(&t, &y, f, 0, 4 * PI, -1, 1);

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

double f(double t, double y)
{
    return sin(t);
}