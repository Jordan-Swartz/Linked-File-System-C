#ifndef STACK_H
#define STACK_H

/*Type Definitions*/
typedef struct FSNode FSNode;
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