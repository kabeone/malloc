#ifndef MALLOC_STRUCT_MALLOC_H
#define MALLOC_STRUCT_MALLOC_H

typedef char memory_state;

enum {
    mem_free = 0,
    mem_allocated = 1,
};
typedef struct {
    memory_state state;
    size_t size;
} block_stat;

#endif //MALLOC_STRUCT_MALLOC_H
