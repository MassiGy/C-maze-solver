#include "_heap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct _elt {
    struct _elt *next, *prev;
    void *elt;
};

struct _elt* new_elt(void *content) {
    struct _elt* _new = malloc(sizeof(struct _elt));
    _new -> elt = content;
    return _new;
}

typedef struct _heap {
    struct _elt *head, *end;
} Heap;

Heap* empty_heap() {
    Heap *heap = malloc(sizeof(Heap));
    heap -> head = heap -> end = NULL;
    return heap;
}

void* heap_top(Heap const *heap) {
    if (heap_is_empty(heap)) {
        fprintf(stderr, "File vide\n");
        exit(EXIT_FAILURE);
    }
    return heap -> end -> elt;
}

bool heap_is_empty(Heap const *heap) {
    return heap -> end == NULL && heap -> head == NULL;
}

void heap_add(Heap *heap, void *elt) {
    struct _elt * _new = new_elt(elt);
    if (heap_is_empty(heap)) {
        heap -> end = heap -> head = _new;
        return;
    }
    _new -> next = heap -> head -> elt;
    heap -> head -> prev = _new;
    heap -> head = _new;
}

void* heap_pop(Heap *heap) {
    if (heap_is_empty(heap)) {
        fprintf(stderr, "File vide\n");
        exit(EXIT_FAILURE);
    }
    void *res = heap -> end -> elt;
    struct _elt *last = heap -> end;
    if (last == heap -> head) {
        heap -> end = heap ->head = NULL;
    } else {
        heap -> end = last -> prev;
    }
    free(last);
    return res;
}

void delete_heap(Heap *heap) {
    while (!heap_is_empty(heap)) {
        heap_pop(heap);
    }
    free(heap);
} 