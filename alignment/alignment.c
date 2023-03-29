#include "alignment.h"

#include <stddef.h>

size_t align(size_t size)
{
    size_t result;
    size_t res = size / sizeof(long double);
    if (size % sizeof(long double) != 0)
    {
        res++;
    }
    if (__builtin_mul_overflow(res, sizeof(long double), &result))
    {
        return 0;
    }
    return result;
}
