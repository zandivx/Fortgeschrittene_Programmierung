#include <stddef.h>
#include "../numericalodesc/matrix.h"

int main()
{
    matrix m = {NULL, 5, 2};
    create_m(&m);

    int val = 0;
    for (size_t i = 0; i < m.r; i++)
    {
        for (size_t j = 0; j < m.c; j++)
        {
            set(m, i, j, val);
            val++;
        }
    }

    print_m(m);
    transpose(&m);
    print_m(m);
    transpose(&m);
    print_m(m);

    return 0;
}