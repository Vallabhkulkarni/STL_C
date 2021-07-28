#ifndef C_DATASTRUCTURES_STACK_H
#define C_DATASTRUCTURES_STACK_H

#ifdef __cplusplus
extern "C" {
#endif



typedef struct Stack {
    void **memory;
    int allocated;
    int top;

    void (*freeItem)(void *);
} Stack;

Stack *stackInitialization(void (*freeItem)(void *item));

void stackPush(Stack *stack, void *data);

void stackAddAll(Stack *stack, void **array, int arrLength);

void *stackPop(Stack *stack);

short stackIsEmpty(Stack *stack);

void *stackPeek(Stack *stack);

void **stackToArray(Stack *stack);

int stackGetLength(Stack *stack);

int stackContains(Stack *stack, void *item, int (*cmp)(const void *, const void *));

int stackEquals(Stack *stack, Stack *stack1, int (*cmp)(const void *, const void *));

void clearStack(Stack *stack);

void destroyStack(void *stack);

#ifdef __cplusplus
}
#endif

#endif 
