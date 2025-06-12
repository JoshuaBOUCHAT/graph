#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

queue *queue_with_capacity(queue *p_queue, int capacity) {
  p_queue->inner = malloc(sizeof(int) * (capacity + 1));
  if (p_queue->inner == NULL) {
    return NULL;
  }
  p_queue->size = capacity + 1;
  p_queue->head = 0;
  p_queue->tail = 0;

  return p_queue;
}
bool is_empty(queue *p_queue) { return p_queue->head == p_queue->tail; }
bool is_full(queue *p_queue) {
  return (p_queue->head + 1) % p_queue->size == p_queue->tail;
}

bool enqueue(queue *p_queue, int data) {
  if (is_full(p_queue)) {
    printf("Try to enqueue while the queue is full !!\n");
    return true;
  }
  p_queue->inner[p_queue->head] = data;

  p_queue->head =
      ((p_queue->head + 1) == p_queue->size) ? 0 : p_queue->head + 1;

  return false;
}
int dequeue(queue *p_queue) {
  if (is_empty(p_queue)) {
    printf("Try to dequeue while the queue is empty !!\n");
    return -1;
  }
  int data = p_queue->inner[p_queue->tail];

  p_queue->tail =
      ((p_queue->tail + 1) == p_queue->size) ? 0 : p_queue->tail + 1;

  return data;
}
void free_queue(queue *p_queue) {
  free(p_queue->inner);
  p_queue->inner = NULL;
  p_queue->size = 0;
  p_queue->head = 0;
  p_queue->tail = 0;
}
