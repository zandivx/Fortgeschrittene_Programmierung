#include "broadcast.h"
#include <stddef.h>

void broadcast_add(double *dest, double *src, size_t n, double term)
{
    for (size_t i = 0; i < n; i++)
    {
        dest[i] = src[i] + term;
    }
}