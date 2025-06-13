#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

heap *heap_with_capacity(heap *p_heap, int capacity)
{
    bool pre_allocated = true;
    if (p_heap == NULL)
    {
        p_heap = malloc(sizeof(heap));
        if (p_heap == NULL)
        {
            return NULL;
        }
        pre_allocated = false;
    }

    p_heap->inner = malloc(sizeof(heap_node) * capacity);
    if (p_heap->inner == NULL)
    {
        if (!pre_allocated)
        {
            free(p_heap);
        }
        return NULL;
    }
    p_heap->nb_node = 0;
    p_heap->capacity = capacity;

    return p_heap;
}
bool swap_if_greater(heap *p_heap, int root, int min)
{
    if (p_heap->inner[root].distance < p_heap->inner[min].distance)
    {
        return false;
    }

    heap_node temp = p_heap->inner[root];
    p_heap->inner[root] = p_heap->inner[min];
    p_heap->inner[min] = temp;
    return true;
}

void sift_down(heap *p_heap, int root)
{
    if (p_heap == NULL)
    {
        printf("Tentative de siftdown sur heap null !\n");
        exit(EXIT_FAILURE);
    }
    if (p_heap->nb_node<2){
        return;
    }
    if ( root >= p_heap->nb_node)
    {
        printf("Sift down appeler un mauvais root: %d, nb_node:%d", root, p_heap->nb_node);
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        int left = (root << 1) + 1;
        if (left >= p_heap->nb_node)
        {
            return;
        }
        int right = left + 1;
        if (right == p_heap->nb_node)
        {
            swap_if_greater(p_heap, root, left);
            return;
        }
        int min = (p_heap->inner[right].distance < p_heap->inner[left].distance) ? right : left;

        if (swap_if_greater(p_heap, root, min))
        {
            // exchange needed so we have to bubble down
            root = min;
            continue;
        }
        return;
    }
}
void sift_up(heap *p_heap, int root)
{
    if (root < 1)
    {
        return;
    }
    while (root > 0)
    {

        int parent = (root - 1) >> 1;
        if (!swap_if_greater(p_heap, parent, root))
        {
            return;
        }
        root = parent;
    }
}

bool is_heap_full(heap *p_heap)
{
    if (p_heap == NULL)
    {
        printf("Tentative de is_full sur heap NULL !\n");
        exit(EXIT_FAILURE);
    }
    return p_heap->nb_node == p_heap->capacity;
}
bool is_heap_empty(heap *p_heap)
{
    if (p_heap == NULL)
    {
        printf("Tentative de is_empty sur heap NULL !\n");
        exit(EXIT_FAILURE);
    }
    return p_heap->nb_node == 0;
}

heap_node pop(heap *p_heap)
{
    if (p_heap == NULL)
    {
        printf("Le program a essayer de pop sur in heap NULL !\n");
        exit(EXIT_FAILURE);
    }
    if (is_heap_empty(p_heap))
    {
        printf("Le program a essayer de pop sur un heap vide !\n");
        exit(EXIT_FAILURE);
    }
    heap_node temp = p_heap->inner[0];
    p_heap->inner[0] = p_heap->inner[--p_heap->nb_node];
    sift_down(p_heap, 0);
    return temp;
}
heap_node *upgrade_capacity(heap *p_heap)
{
    int capacity = p_heap->capacity;
    int temp = capacity >> 1;
    int new_capacity = capacity + temp + (temp >> 2);
    p_heap->inner = realloc(p_heap->inner, sizeof(heap_node) * new_capacity);
    return p_heap->inner;
}

void push(heap *p_heap, heap_node node)
{
    if (p_heap == NULL)
    {
        printf("Le program a essayer de push sur in heap NULL !\n");
        exit(EXIT_FAILURE);
    }
    if (is_heap_full(p_heap))
    {
        if (upgrade_capacity(p_heap) == NULL)
        {
            printf("Impossible de resize le heap !\n");
            exit(EXIT_FAILURE);
        }
    }
    p_heap->inner[p_heap->nb_node] = node;
    sift_up(p_heap, p_heap->nb_node);
    p_heap->nb_node++;
}
void push_from_value(heap *p_heap, int index, double distance)
{
    heap_node node = {
        .index = index,
        .distance = distance,
    };
    push(p_heap, node);
}
void free_heap(heap *p_heap)
{
    free(p_heap->inner);
}