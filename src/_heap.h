#pragma once
#include <stdbool.h>

typedef struct _heap Heap;

Heap * empty_heap();

void* heap_top(Heap const*);

bool heap_is_empty(Heap const*);

void heap_add(Heap *, void*);

void* heap_pop(Heap *);

void delete_heap(Heap *);