/**
 * @file stack.h
 * @brief Defines the structs and methods for a basic stack implementation.
 *
 * This header file provides function declarations and struct definitions for a
 * simple stack implementation. The stack is used to facilitate depth-first
 * search (DFS) and other hierarchical operations in the file system.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025-02-20
 */

#ifndef STACK_H
#define STACK_H

//max size to prevent overflow
#define STACK_MAX_SIZE 1000

/*Type Definitions*/
typedef struct FSNode FSNode;
typedef struct Stack Stack;

/**
 * @struct Stack
 * @brief Represents a stack.
 */
struct Stack {
    FSNode* nodes[STACK_MAX_SIZE];
    int top;
};

/*Function Declarations*/
/**
 * Initializes a stack.
 *
 * @param stack Pointer to the stack.
 */
void init_stack(Stack* stack);

/**
 * Checks if the stack is empty.
 *
 * @param stack Pointer to the stack.
 * @return Returns 1 if the stack is empty, otherwise 0.
 */
int is_empty(Stack* stack);

/**
 * Pushes an FSNode pointer onto the stack.
 *
 * @param stack Pointer to the stack.
 * @param node Node to push onto the stack.
 * @return Returns 1 if success, otherwise -1 for stack overflow.
 */
int push(Stack* stack, FSNode* node);

/**
 * Pops an FSNode pointer from the stack.
 *
 * @param stack
 * @return Pointer to the FSNode that was removed from the stack.
 */
FSNode* pop(Stack* stack);

/**
 * Peeks at the top FSNode of the stack without removing it.
 *
 * @param stack
 * @return Pointer to the FSNode at the top of the stack.
 */
FSNode* peek(Stack* stack);

#endif