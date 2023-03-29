#include "recycler.h"

#include <stdlib.h>

struct recycler *recycler_create(size_t block_size, size_t total_size)
{
    if (block_size == 0 || total_size == 0 || total_size % block_size != 0)
    {
        return NULL;
    }
    if (block_size % sizeof(size_t) != 0)
    {
        return NULL;
    }
    struct recycler *new = malloc(sizeof(struct recycler));
    if (!new)
    {
        return NULL;
    }
    new->chunk = NULL;
    new->block_size = block_size;
    new->capacity = total_size;
    return new;
}
void recycler_destroy(struct recycler *r)
{
    if (r != NULL)
    {
        r->block_size = 0;
        r->capacity = 0;
        free(r->chunk);
        free(r);
    }
}

void *recycler_allocate(struct recycler *r)
{
    if (r != NULL && r->free != NULL)
    {
        void *free_head = r->free;
        struct free_list *f_list = (struct free_list *)r->free;
        if (f_list->next != NULL)
        {
            f_list = f_list->next;
        }
        else
        {
            f_list = NULL;
        }
        r->free = (void *)f_list;
        return free_head;
    }
    return NULL;
}
void recycler_free(struct recycler *r, void *block)
{
    if (!(r == NULL || block == NULL))
    {
        struct free_list *free_list2 = (struct free_list *)block;
        free_list2->next = (struct free_list *)r->free;
        r->free = (void *)free_list2;
    }
}
