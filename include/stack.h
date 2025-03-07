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
typedef enum { STACK_NODE, STACK_STRING } StackItemType;
typedef struct Stack Stack;
typedef struct StackItem StackItem;

/**
 * @struct StackItem
 * @brief Represents an item in the stack.
 */
struct StackItem {
    StackItemType type;     //type of item
    void* data;             //pointer to item
};


/**
 * @struct Stack
 * @brief Represents a stack.
 */
struct Stack {
    StackItem items[STACK_MAX_SIZE];
    int top;
};


/*Function Declarations*/
/**
 * Initializes a generic stack.
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
 * Pushes an item pointer onto the stack.
 *
 * @param stack Pointer to the stack.
 * @param item Item to push onto the stack.
 * @param type The type of item being added (STACK_NODE or STACK_STRING)
 * @return Returns 1 if success, otherwise -1 for stack overflow.
 */
int push(Stack* stack, void* item, StackItemType type);

/**
 * Pops an item pointer from the stack.
 *
 * @param stack
 * @return Pointer to the item that was removed from the stack.
 */
void* pop(Stack* stack);

/**
 * Peeks at the top item of the stack without removing it.
 *
 * @param stack
 * @return Pointer to the FSNode at the top of the stack.
 */
void* peek(Stack* stack);

#endif