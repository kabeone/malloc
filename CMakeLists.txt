cmake_minimum_required(VERSION 3.15)
project(malloc C)

set(CMAKE_C_STANDARD 11)

add_executable(malloc
        malloc.c header/struct_malloc.h src/malloc_function.c header/malloc_function.h)
