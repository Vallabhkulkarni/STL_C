#ifndef C_DATASTRUCTURES_DLINKEDLISTDEQUE_H
#define C_DATASTRUCTURES_DLINKEDLISTDEQUE_H

#ifdef __cplusplus
extern "C" {
#endif




typedef struct DLDeque {

    struct DoublyLinkedList *linkedList;

} DLDeque;


DLDeque *dlDequeInitialization(void (*freeFun)(void *));

void dLDequeInsertFront(DLDeque *deque, void *item);

void dLDequeInsertRear(DLDeque *deque, void *item);

void *dLDequeGetFront(DLDeque *deque);

void *dLDequeGetRear(DLDeque *deque);

void *dLDequePeekFront(DLDeque *deque);

void *dLDequePeekRear(DLDeque *deque);

void **dLDequeToArray(DLDeque *deque);

int dLDequeGetLength(DLDeque *deque);

int dLDequeIsEmpty(DLDeque *deque);

void clearDLDeque(DLDeque *deque);

void destroyDLDeque(void *deque);

#ifdef __cplusplus
}
#endif

#endif
