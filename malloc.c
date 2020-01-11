#include <unistd.h>
#include <stdio.h>
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

int main()
{
    int *a;
    char *str = my_malloc(100);
    block_stat *block = (block_stat *) (str - sizeof(block_stat));


    printf("%zu\n", sizeof(block_stat));
    printf("size 1st block %d\n", block->size);
    block += block->size + sizeof(block_stat);
    printf("empty space after block %d\n", block->size);

    for (int i = 0; i < 100; i++)
        str[i] = 'A';
    write(1, str, 100);
    write(1, "\n", 1);
    my_free(str, 4096);

    return 0;
}