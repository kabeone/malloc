#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "header/malloc_function.h"
#include "header/struct_malloc.h"



void my_free(void *ptr, int size)
{
    if (ptr == NULL)
        return;
    if (munmap(ptr - sizeof(block_stat), size) == -1)
        printf("Error : free : memory at address `%p` was not allocated\n", ptr);
}

void *my_realloc(void *ptr, size_t size)
{
    return NULL;
}

int main(int argc, char ** argv)
{
    int size;
    char *str;
    //block_stat *block;

    if (argc != 2) {
        printf("Error : wrong nb of arguments\n");
        return 84;
    }
    size = atoi(argv[1]);

    for (int i = 0; i < 2050; i++)
        str = my_malloc(size);

    return 0;
}