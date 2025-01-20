//
// Created by jordan on 1/19/25.
//

#ifndef STACK_H
#define STACK_H

#include "file_system.h"

/*Type Definitions*/
typedef struct Stack Stack;

struct Stack {
    FSNode* nodes[1000];
    int top;
};

void init_stack(Stack* stack);
int is_empty(Stack* stack);
void push(Stack* stack, FSNode* node);
FSNode* pop(Stack* stack);
FSNode* peek(Stack* stack);

#endif