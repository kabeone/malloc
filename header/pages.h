#ifndef MALLOC_PAGES_H
#define MALLOC_PAGES_H

void *page_ref();

void *realloc_page_ref(int nb_pages, void *ptr);

#endif
