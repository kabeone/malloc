#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
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

void write_nb(char *str, int nb){
    write(1, str, nb);
}


int main(int argc, char ** argv)
{
    char *str;
    char *str2;
    char *str3;
    char *str4;
    char *str5;
    for (int i = 0; i < 1000000; i++) {
        str3 = my_malloc(15);
        //str = my_malloc(100);
        //str2 = my_malloc(255);


        //str4 = my_malloc(2000);
        // à corriger

        //str5 = my_malloc(4000);
        // semble okay

        printf("%d\n", i);
    }

    return 0;
}