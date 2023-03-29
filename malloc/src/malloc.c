#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

void *base = NULL;

struct meta
{
    // 0 if not free
    // 1 if free
    size_t status;
    size_t size;
    struct meta *next;
    size_t vide;
    char data[];
};

size_t align(size_t size, size_t to_align)
{
    size_t a = size / to_align;
    size_t res;
    if (size % to_align != 0)
    {
        a++;
    }
    if (__builtin_mul_overflow(a, to_align, &res))
    {
        return 0;
    }
    return res;
}
struct meta *extend(struct meta *last, size_t size)
{
    size_t pages = align(size + sizeof(struct meta), sysconf(_SC_PAGESIZE));
    struct meta *block = mmap(NULL, pages, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED)
    {
        return NULL;
    }
    block->status = 0;
    block->size = pages - sizeof(struct meta);
    if (last != NULL)
    {
        last->next = block;
    }
    return block;
}
struct meta *find_block(struct meta **last, size_t size)
{
    struct meta *begin = base;
    while (begin && !(begin->status && begin->size >= size))
    {
        *last = begin;
        begin = begin->next;
    }
    return begin;
}

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    size = align(size, sizeof(long double));
    if (base == NULL)
    {
        // 1er appel
        struct meta *first = extend(NULL, size);
        if (!first)
        {
            return NULL;
        }
        base = first;
        struct meta *rest;
        rest = (struct meta *)(first->data + size);
        rest->next = first->next;
        rest->size = first->size - size - sizeof(struct meta);
        rest->status = 1;
        first->size = size;
        first->next = rest;
        return first->data;
    }

    // autres appels
    struct meta *last = base;
    struct meta *block = find_block(&last, size);

    if (block)
    {
        if (block->size - size >= sizeof(struct meta) + sizeof(long double))
        {
            // split_block
            struct meta *new;
            new = (struct meta *)(block->data + size);
            new->size = block->size - size - sizeof(struct meta);
            new->next = block->next;
            new->status = 1;
            block->size = size;
            block->next = new;
        }
        block->status = 0;
    }
    else
    {
        block = extend(last, size);
        if (!block)
            return NULL;
    }
    return block->data;
}

__attribute__((visibility("default"))) void free(void *ptr)
{
    (void)ptr;
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    (void)ptr;
    (void)size;
    return NULL;
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    size_t tmp;
    if (__builtin_mul_overflow(nmemb, size, &tmp))
        return NULL;
    char *new = malloc(nmemb * size);
    if (new)
    {
        for (size_t i = 0; i < nmemb * size; i++)
        {
            new[i] = 0;
        }
        // void *new_void = new;
        return (void *)new;
    }
    return NULL;
}
