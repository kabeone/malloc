#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "../header/struct_malloc.h"

#define page_size getpagesize()
#define TINY 256
#define SMALL 2048
#define tiny_zone page_size * (TINY * 100 / page_size + 1)
#define small_zone page_size * (SMALL * 100 / page_size + 1)

void *page_array[2] = { NULL, NULL };

static void *init_space(size_t size, void *ptr)
{
    block_stat *block = ptr;
    int empty_space = block->size;
    type_zone type = small;

    if (size <= TINY)
        type = tiny;

    block->state = mem_allocated;
    block->size = size;
    block->type = type;

    block = ptr + size + sizeof(block_stat);

    block->state = mem_free;
    block->size = empty_space - size;
    block->type = type;

    return ptr + sizeof(block_stat);
}

static void *check_space(size_t size, void *ptr)
{
    block_stat * block = ptr;
    unsigned long mem_position = 0;
    int size_zone = small_zone;

    if (size <= TINY)
        size_zone = tiny_zone;

    while (block->state != mem_free) {
        mem_position += block->size + sizeof(block_stat);
        block = (block_stat *) ((char *) block + block->size + sizeof(block_stat));

        if (size_zone - mem_position < size + sizeof(block_stat))
            return NULL;
    }

    while (block->size < size + sizeof(block_stat)) {
        do {
            mem_position += block->size + sizeof(block_stat);
            block = (block_stat *) ((char *) block + block->size + sizeof(block_stat));

            if (size_zone - mem_position < size + sizeof(block_stat))
                return NULL;

        } while (block->state != mem_free);
    }

    return init_space(size, (void *) block);
}

static void *alloc(size_t size)
{
    void *ptr;
    size_t asked_size = size;
    block_stat *block;
    unsigned long nb_page = 0;
    type_zone type = 0;

    if (size > SMALL) {
        while (page_size * nb_page < size + sizeof(block_stat))
            nb_page++;
        size = nb_page * page_size;
        type = large;
    } else if (size <= TINY) {
        size = tiny_zone;
        type = tiny;
    } else if (size <= SMALL) {
        size = small_zone;
        type = small;
    }

    if ( (ptr = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == (void *) -1)
        return NULL;
    block = ptr;
    block->state = mem_allocated;
    block->size = asked_size;
    block->type = type;
    if (asked_size <= SMALL) {
        block = ptr + asked_size + sizeof(block_stat);
        block->state = mem_free;
        block->size = size - (2 * sizeof(block_stat) + asked_size);
        block->type = type;
    }
    return ptr += sizeof(block_stat);
}

void *my_malloc(size_t size)
{
    void *ptr = NULL;

    if (size == 0)
        return NULL;
    if (size <= TINY) {
        if (page_array[0]) {
            check_space(size, page_array[0]);
        } else {
            ptr = alloc(size);
            *page_array = ptr - sizeof(block_stat);
        }
    } else if (size <= SMALL)
        if (page_array[1]) {
            check_space(size, page_array[1]) ;
        } else {
            ptr = alloc(size);
            page_array[1] = ptr - sizeof(block_stat);
        }
    else
        ptr = alloc(size);
    return ptr;
}