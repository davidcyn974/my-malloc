#include "allocator.h"

#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "utils.h"

struct blk_allocator *blka_new(void)
{
    struct blk_allocator *new_allocator = malloc(sizeof(struct blk_allocator));
    if (!new_allocator)
    {
        return NULL;
    }
    new_allocator->meta = NULL;
    return new_allocator;
}

size_t align(size_t size)
{
    size_t result;
    size_t taille_totale = sysconf(_SC_PAGE_SIZE);
    size_t taille = size / taille_totale;
    if (size % taille_totale != 0)
    {
        taille++;
    }
    if (__builtin_mul_overflow(taille, taille_totale, &result))
    {
        return 0;
    }
    return result;
}

struct blk_meta *blka_alloc(struct blk_allocator *blka, size_t size)
{
    size = align(size + sizeof(struct blk_meta));
    struct blk_meta *block = mmap(NULL, size, PROT_READ | PROT_WRITE,
                                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED)
        return NULL;
    if (blka->meta == NULL)
    {
        // on gere le nouveau meta.
        block->size = size - sizeof(struct blk_meta);
        block->next = NULL;
        // alloue le nouveau meta a Allocator.
        blka->meta = block;
        return block;
    }
    else
    {
        // on gere le nouveau meta.
        block->size = size - sizeof(struct blk_meta);
        block->next = blka->meta;
        // alloue le nouveau 1er meta a Allocator.
        blka->meta = block;
        return block;
    }
}
void blka_free(struct blk_meta *blk)
{
    munmap(blk, blk->size);
}
void blka_pop(struct blk_allocator *blka)
{
    if (blka && blka->meta)
    {
        struct blk_meta *next = blka->meta->next;
        blka_free(blka->meta);
        blka->meta = next;
    }
}

void blka_delete(struct blk_allocator *blka)
{
    if (blka->meta)
    {
        blka_free(blka->meta);
    }
    free(blka);
}
