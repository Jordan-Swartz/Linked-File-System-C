//
// Created by jordan on 1/19/25.
//

#include "stack.h"
#include <stddef.h>

void init_stack(Stack* stack) {
    stack->top = -1;
}

int is_empty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, FSNode* node) {
    stack->nodes[++stack->top] = node;
}

FSNode* pop(Stack* stack) {
    if (is_empty(stack)) return NULL;
    return stack->nodes[stack->top--];
}

FSNode* peek(Stack* stack) {
    if (is_empty(stack)) return NULL;
    return stack->nodes[stack->top];
}