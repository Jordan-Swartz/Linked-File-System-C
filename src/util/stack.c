#include "stack.h"
#include <stddef.h>

void init_stack(Stack* stack) {
    stack->top = -1;
}

int is_empty(Stack* stack) {
    return stack->top == -1;
}

int push(Stack* stack, FSNode* node) {
    if (stack->top >= STACK_MAX_SIZE - 1) {
        return -1;  //overflow
    }
    stack->nodes[++stack->top] = node;
    return 1;
}

FSNode* pop(Stack* stack) {
    if (is_empty(stack)) return NULL;
    return stack->nodes[stack->top--];
}

FSNode* peek(Stack* stack) {
    if (is_empty(stack)) return NULL;
    return stack->nodes[stack->top];
}