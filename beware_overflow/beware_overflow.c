#include <stddef.h>

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    size_t res = 0;
    if (__builtin_mul_overflow(nmemb, size, &res))
    {
        return NULL;
    }
    char *c = ptr;
    c += res;
    return c;
}
