#include "../Headers/DLinkedListDeque.h"
#include "../../LinkedLists/Headers/DoublyLinkedList.h"
#include "../../../System/Utils.h"
#include "../../../Unit Test/CuTest/CuTest.h"


/** This function will initialize a new doubly linked list deque in the memory,
 * then return it's pointer.
 */

DLDeque *dlDequeInitialization(void (*freeFun)(void *)) {

    if (freeFun == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return NULL;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "freeing function pointer", "doubly linked list deque data structure");
        exit(INVALID_ARG);
#endif

    }

    DLDeque *dlDeque = (DLDeque *) malloc(sizeof(DLDeque));
    if (dlDeque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = FAILED_ALLOCATION;
        return NULL;
#else
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "doubly linked list deque",
                "doubly linked list deque data structure");
        exit(FAILED_ALLOCATION);
#endif

    }
    dlDeque->linkedList = doublyLinkedListInitialization(freeFun, NULL);

    return dlDeque;

}


/** This function will take a new item then,
 * it will insert the passed item in the front of the deque.
 */

void dLDequeInsertFront(DLDeque *deque, void *item) {
    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    } else if (item == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "new item pointer", "doubly linked list deque data structure");
        exit(INVALID_ARG);
#endif

    }

    doublyLinkedListAddFirst(deque->linkedList, item);

}


/** This function will take a new item,
 * then it will insert the new item in the end of the deque.
 */

void dLDequeInsertRear(DLDeque *deque, void *item) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    } else if (item == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = INVALID_ARG;
        return;
#else
        fprintf(stderr, INVALID_ARG_MESSAGE, "new item pointer", "doubly linked list deque data structure");
        exit(INVALID_ARG);
#endif

    }

    doublyLinkedListAddLast(deque->linkedList, item);

}


/** This function will return the front item in the deque,
 * then it will remove the item from the deque.
 */

void *dLDequeGetFront(DLDeque *deque) {
    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    } else if (dLDequeIsEmpty(deque)) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = EMPTY_DATA_STRUCTURE;
        return NULL;
#else
        fprintf(stderr, EMPTY_DATA_STRUCTURE_MESSAGE, "doubly linked list deque data structure");
        exit(EMPTY_DATA_STRUCTURE);
#endif

    }

    return doublyLinkedListDeleteFirstWtoFr(deque->linkedList);

}


/** This function will return the last item in the deque,
 * then it will remove the item from the deque.
 */

void *dLDequeGetRear(DLDeque *deque) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    } else if (dLDequeIsEmpty(deque)) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = EMPTY_DATA_STRUCTURE;
        return NULL;
#else
        fprintf(stderr, EMPTY_DATA_STRUCTURE_MESSAGE, "doubly linked list deque data structure");
        exit(EMPTY_DATA_STRUCTURE);
#endif

    }

    return doublyLinkedListDeleteLastWtoFr(deque->linkedList);

}


/** This function will return the first item in the deque,
 * without removing the item from the deque.
 */

void *dLDequePeekFront(DLDeque *deque) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    } else if (dLDequeIsEmpty(deque)) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = EMPTY_DATA_STRUCTURE;
        return NULL;
#else
        fprintf(stderr, EMPTY_DATA_STRUCTURE_MESSAGE, "doubly linked list deque data structure");
        exit(EMPTY_DATA_STRUCTURE);
#endif

    }

    return doublyLinkedListGetFirst(deque->linkedList);

}


/** This function will return the last item in the deque,
 * without removing the item from the deque.
 */

void *dLDequePeekRear(DLDeque *deque) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    } else if (dLDequeIsEmpty(deque)) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = EMPTY_DATA_STRUCTURE;
        return NULL;
#else
        fprintf(stderr, EMPTY_DATA_STRUCTURE_MESSAGE, "doubly linked list deque data structure");
        exit(EMPTY_DATA_STRUCTURE);
#endif

    }

    return doublyLinkedListGetLast(deque->linkedList);

}


/** This function will return a double void array that consist of the deque items.
 */

void **dLDequeToArray(DLDeque *deque) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return NULL;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    }

    return doublyLinkedListToArray(deque->linkedList);

}


/** This function will return the number of the items in the deque.
 */

int dLDequeGetLength(DLDeque *deque) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return -1;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    }

    return doublyLinkedListGetLength(deque->linkedList);

}


/** This function will check if the deque is empty or not,
 * and if it was the function will return one (1), other wise it will return zero (0).
 */

int dLDequeIsEmpty(DLDeque *deque) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return -1;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    }

    return doublyLinkedListIsEmpty(deque->linkedList);

}


/** This function will clear and free all the deque items,
 * without freeing the deque.
 */

void clearDLDeque(DLDeque *deque) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    }

    doublyLinkedListClear(deque->linkedList);

}


/** This function will destroy and free the deque with all it's items.
 */

void destroyDLDeque(void *deque) {

    if (deque == NULL) {
#ifdef C_DATASTRUCTURES_ERRORSTESTSTRUCT_H
        ERROR_TEST->errorCode = NULL_POINTER;
        return;
#else
        fprintf(stderr, NULL_POINTER_MESSAGE, "deque", "doubly linked list deque data structure");
        exit(NULL_POINTER);
#endif

    }

    destroyDoublyLinkedList(((DLDeque *) deque)->linkedList);
    free(deque);

}