#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "../header/struct_malloc.h"
#include "../header/pages.h"

#define page_size getpagesize()
#define TINY 256
#define SMALL 2048
#define tiny_zone page_size * (TINY * 100 / page_size + 1)
#define small_zone page_size * (SMALL * 100 / page_size + 1)

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
    block->size = empty_space - ( size + sizeof(block_stat));
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

    return ptr + sizeof(block_stat);
}

static void *set_pages_ref(int *_nb_pages, void *page, size_t size, void **ptr)
{
    for (unsigned long i = 0; i < *_nb_pages * page_size / sizeof(void *); i++) {
        if ( ((char **) page)[i] != NULL)
            if ( (*ptr = check_space(size, ((char **) page)[i])) != NULL)
                return page;
    }

    for (unsigned long i = 0; i < *_nb_pages * page_size / sizeof(void *); i++) {

        if ( ((char **) page)[i] == NULL) {
            *ptr = alloc(size);
            ((char **) page)[i] = *ptr - sizeof(block_stat);
            return page;
        }
    }

    page = realloc_page_ref(*_nb_pages, page);

    *ptr = alloc(size);
    ((char **) page)[nb_pages[2] * page_size / sizeof(void *)] = *ptr - sizeof(block_stat);

    *_nb_pages *= 2;

    return page;
}

static void *set_pages_ref_large(void *page, void *size_alloc)
{
    if (size_alloc == NULL)
        return page;

    for (unsigned long i = 0; i < page_size / sizeof(void *) * nb_pages[2]; i++) {
        if (((char **) page)[i] == NULL) {
            ((char **) page)[i] = size_alloc;
            return page;
        }
    }

    page = realloc_page_ref(nb_pages[2], page);

    ((char **) page)[nb_pages[2] * page_size / sizeof(void *)] = size_alloc;
    nb_pages[2] *= 2;

    return page;
}

void *my_malloc(size_t size)
{
    void *ptr = NULL;
    char **page;

    if (size == 0)
        return NULL;

    if (size <= TINY) {
        if (page_array[0]) {
            page_array[0] = set_pages_ref(&nb_pages[0], page_array[0], size, &ptr);
        } else {
            page = page_ref();
            page[0] = ptr = alloc(size);
            page[0] -= sizeof(block_stat);
            page_array[0] = page;
            nb_pages[0]++;
        }

    } else if (size <= SMALL)
        if (page_array[1]) {
            page_array[1] = set_pages_ref(&nb_pages[1], page_array[1], size, &ptr);
        } else {
            page = page_ref();
            page[1] = ptr = alloc(size);
            page[1] -= sizeof(block_stat);
            page_array[1] = page;
            nb_pages[1]++;
        }

    else {
        if (page_array[2] == NULL) {
            page = page_ref();
            page_array[2] = page;
            nb_pages[2]++;
        }
        ptr = alloc(size);
        page_array[2] = set_pages_ref_large(page_array[2], ptr - sizeof(block_stat));
    }
    return ptr;
}