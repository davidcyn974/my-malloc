#include <stddef.h>

union Union
{
    size_t start;
    size_t *ptr;
};

void *page_begin(void *ptr, size_t page_size)
{
    if (page_size == 0)
    {
        return NULL;
    }
    union Union res;
    res.ptr = ptr;
    void *begin = (void *)(res.start & ~(page_size - 1));
    return begin;
}
