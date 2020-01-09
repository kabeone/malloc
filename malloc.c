#include <stdlib.h>

void *malloc(size_t size)
{
    return NULL;
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;
}

void *realloc(void *ptr, size_t size)
{
    return NULL;
}

int main()
{
    return 0;
}