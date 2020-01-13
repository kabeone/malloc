#ifndef MALLOC_STRUCT_MALLOC_H
#define MALLOC_STRUCT_MALLOC_H

static void *page_array[] = { NULL, NULL, NULL };
static int nb_pages[] = {0, 0, 0};

typedef char memory_state;
typedef char type_zone;

enum {
    mem_free = 0,
    mem_allocated = 1,
};

enum {
    tiny,
    small,
    large,
};

typedef struct {
    memory_state state;
    size_t size;
    type_zone type;
} block_stat;

#endif //MALLOC_STRUCT_MALLOC_H
