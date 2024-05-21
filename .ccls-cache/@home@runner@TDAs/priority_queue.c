#include "priority_queue.h"
#include "list.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>

int _int_lower_than(void *key1, void *key2) {
  return *((int *)key1) < *((int *)key2);
}

PQueue *pqueue_create(PQueue *queue) {
  return sorted_map_create(_int_lower_than);
}

void pqueue_insert(PQueue *queue, int priority, void *data) {
  int *priority_ptr = (int *)malloc(sizeof(int));
  *priority_ptr = -priority;
  map_insert(queue, priority_ptr, data);
}

void *pqueue_remove(PQueue *queue) {
  MapPair *pair = list_popFront((List *)queue);
  free(pair->key);
  return pair->value;
}

void *pqueue_front(PQueue *queue) {
  MapPair *pair = list_first((List *)queue);
  return pair->value;
}

void pqueue_clean(PQueue *queue) { list_clean((List *)queue); }