#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

#define page_size getpagesize()

void *malloc(size_t size)
{
    void *ptr;

    if ( (ptr = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == (void *) -1)
        return NULL;
    return ptr;
}

void free(void *ptr, int size)
{
    if (ptr == NULL)
        return;
    if (munmap(ptr, size) == -1)
        printf("Error : free : memory at address `%p` was not allocated\n", ptr);
}

void *realloc(void *ptr, size_t size)
{
    return NULL;
}

int main()
{
    int *a;
    char *str = malloc(100);

    printf("%p\n", str);

    for (int i = 0; i < 100; i++)
        str[i] = 'A';
    write(1, str, 100);
    write(1, "\n", 1);
    free(str, 100);

    return 0;
}