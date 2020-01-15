#define _GNU_SOURCE
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define page_size getpagesize()

static void print_err(char *str)
{
    write(2, str, strlen(str));
}

void *page_ref()
{
    void *ptr;

    if ( (ptr = mmap(NULL, page_size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == (void *) -1) {
        print_err(strerror(errno));
        print_err("\n");
        exit(84);
    }

    for (int i = 0; i < page_size; i++)
        ((char *)ptr)[i] = 0;

    return ptr;
}

void *realloc_page_ref(int nb_pages, void *ptr)
{
    void *new_ptr;

    if ( (new_ptr = mmap(NULL, nb_pages * page_size * 2, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == (void *) -1) {
        print_err(strerror(errno));
        print_err("\n");
        exit(84);
    }
    memcpy(new_ptr, ptr, nb_pages * page_size);

    munmap(ptr, nb_pages * page_size);

    for (int i = (nb_pages * page_size); i < nb_pages * page_size * 2; i++)
        ((char *)ptr)[i] = 0;

    return new_ptr;
}