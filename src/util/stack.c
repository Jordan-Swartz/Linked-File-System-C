#include "stack.h"
#include <stddef.h>
#include <stdio.h>

void init_stack(Stack* stack) {
    stack->top = -1;
}

int is_empty(Stack* stack) {
    return stack->top == -1;
}

int push(Stack* stack, void* item, StackItemType type) {
    if (stack->top >= STACK_MAX_SIZE - 1) {
        printf("Error: stack overflow");
        return -1;
    }
    stack->top++;
    stack->items[stack->top].type = type;
    stack->items[stack->top].data = item;
    return 1;
}

void* pop(Stack* stack) {
    if (is_empty(stack)) { return NULL; }
    return &stack->items[stack->top--];   //return address of stack item then decrement
}

void* peek(Stack* stack) {
    if (is_empty(stack)) { return NULL; }
    return stack->items[stack->top].data;
}