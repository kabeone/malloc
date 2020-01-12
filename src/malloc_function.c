#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "../header/struct_malloc.h"

#define page_size getpagesize()
#define TINY 256
#define SMALL 2048

static void *page_array[2] = { NULL, NULL };

void *check_space()
{
    return NULL;
}

void *alloc(size_t size)
{
    void *ptr;
    block_stat *block;
    long nb_page = 1;

    if (size > SMALL) {
        ;
    }

    if ( (ptr = mmap(NULL, nb_page * page_size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == (void *) -1)
        return NULL;
    block = ptr;
    block->state = allocated;
    block->size = size;
    if (size <= SMALL) {
        block = ptr + size + sizeof(block_stat);
        block->state = free;
        block->size = page_size - 2 * sizeof(block_stat) - size;
    }
    return ptr += sizeof(block_stat);
}

void *my_malloc(size_t size)
{
    void *ptr = NULL;

    if (size <= TINY) {
        if (*page_array) {}
        else {
            ptr = alloc(size);
            *page_array = ptr - sizeof(block_stat);
        }
    }
    else if (size <= SMALL)
        if (page_array[1]) {}
        else {
            ptr = alloc(size);
            page_array[1] = ptr - sizeof(block_stat);
        }
    else
        ptr = alloc(size);
    return ptr;
}