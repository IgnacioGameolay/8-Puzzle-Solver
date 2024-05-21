#ifndef PQUEUE_H
#define PQUEUE_H
#include "list.h"
#include "map.h"

typedef struct Map PQueue;

PQueue *pqueue_create(PQueue *queue);

void pqueue_insert(PQueue *queue, int priority, void *data);

void *pqueue_remove(PQueue *queue);

void *pqueue_front(PQueue *queue);

void pqueue_clean(PQueue *queue);

#endif /* QUEUE_H */