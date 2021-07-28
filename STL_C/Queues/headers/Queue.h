#ifndef C_DATASTRUCTURES_QUEUE_H
#define C_DATASTRUCTURES_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif



typedef struct Queue {
    void **memory;
    int allocated;
    int front;
    int rear;

    void (*freeItem)(void *);
} Queue;

Queue *queueInitialization(void (*freeFun)(void *));

void queueEnqueue(Queue *arrayQueue, void *data);

void *queueDequeue(Queue *arrayQueue);

void destroyQueue(void *arrayQueue);

void clearQueue(Queue *arrayQueue);

int queueIsEmpty(Queue *arrayQueue);

int queueGetLength(Queue *arrayQueue);

void queueEnqueueAll(Queue *arrayQueue, void **arr, int arrLength);

void *queuePeek(Queue *arrayQueue);

void **queueToArray(Queue *arrayQueue);

#ifdef __cplusplus
}
#endif

#endif 
