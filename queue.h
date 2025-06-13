#ifndef QUEUE_H
#define QUEUE_H

#include "stdbool.h"
#include <stdlib.h>

typedef struct
{
    int *inner;
    int size;
    int head;
    int tail;
} queue;

bool enqueue(queue *p_queue, int data);
int dequeue(queue *p_queue);
/// prepare the queue passed by parameter return back the pointer if everything
/// succeed otherwise return NULL
queue *queue_with_capacity(queue *p_queue, int capacity);

bool is_queue_empty(queue *p_queue);
bool is_queue_full(queue *p_queue);
void free_queue(queue *p_queue);

#endif