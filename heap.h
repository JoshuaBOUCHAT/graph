#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    int index;
    double distance;
} heap_node;

typedef struct
{
    heap_node *inner;
    int nb_node;
    int capacity;
} heap;

heap *heap_with_capacity(heap *p_heap, int capacity);
heap_node pop(heap *p_heap);
void push(heap *p_heap, heap_node node);
bool is_heap_empty(heap *p_heap);
bool is_heap_full(heap *p_heap);
void push_from_value(heap *p_heap, int index, double distance);
void free_heap(heap *p_heap);
#endif