#ifndef MALLOC_STRUCT_MALLOC_H
#define MALLOC_STRUCT_MALLOC_H

typedef char memory_state;

enum {
    free = 0,
    allocated = 1,
};
typedef struct {
    memory_state state;
    int size;
} block_stat;

#endif //MALLOC_STRUCT_MALLOC_H
