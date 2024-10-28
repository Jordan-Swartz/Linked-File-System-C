/**
 * user interface:
 *
 * prompt for commands
 * display outputs
 *
 */

#include <stdlib.h>		//standard lib
#include <stdio.h>		//I/O features
#include <string.h>		//String lib

//header files
#include "ui.h"
#include "file_system.h"
#include "file_operation.h"

int main(void) {
    //prompt if they want to load existing system or create a new one
    system_load();

    //test
    create_node();
    current = current->child_head;
    create_node();
    current = current->child_head;
    create_node();
    current = current->child_head;
    create_node();
    current = current->child_head;
    display_current_path();

    char input;

    do {

    } while (input != 'q');

    system_save();
    return 0;
}

/**
 *
 */
void system_load() {
    //open system file to read
    FILE* file = fopen("", "r");

    if (file == NULL) {
        system_setup();
        return;
    }

    fclose(file);

}

/**
 *
 */
void system_save() {
    //open system file to write
    FILE* file = fopen("", "w");
    fclose(file);
}

/**
 *
 */
void display_current_path() {
    FSNode* iter = current;
    char path[1024] = "";  // Path buffer to store the full path
    char temp[256];        // Temporary buffer for each directory name

    // Traverse upwards from the current node to the root
    while (iter != NULL) {
        // Prepend the current directory name to the path
        sprintf(temp, "/%s", iter->name);
        strcat(temp, path);  // Append the current node to the path
        strcpy(path, temp);  // Copy back the new path

        // Move to the parent node (go upwards)
        iter = iter->parent;
    }

    // Print the full path
    printf("%s\n", path);
}

//Create Stack ADT (class)

//object
struct stack {
    int size;
    Node* head;
};

//create new stack object and return it (constructor)
stack* stack_create() {

}

//destructor to destory object and free memory
void stack_destroy(stack** s) {

}

//add element to stack
void stack_push(stack* s, int e) {

}

//return top element from stack
int stack_pop(stack* s) {

}

//header file for stack class
#indef STACK_H
#define STACK_H

//include files
#include "Stack.h"

//define node struct
typedef struct node node;

struct node {
    void* element;
    node* next;
};

struct stack {
    node* head;
    int size;
};

//functions

//purpose: create a new stack and return it
//return: a pointer to a new stack
stack* stack_create() {
    stack* new_stack = (stack*)malloc(sizeof(stack));

    //memory allocation failed
    if (new_stack == NULL) {
        printf("Error: Failed to create stack.");
        exit(1);
    }
    //memory allocation succeeded
    else {
        new_stack->head = NULL;
        new_stack->size = 0;

        return new_stack;
    }
}

//purpose: destroys a stack
//return: n/a
void stack_destroy(stack** s) {
    node* iter = (*s)->head;

    //free internal list
    while (iter != NULL) {
        node* n = iter->next;
        free(iter);
        iter = n;
    }

    //free stack
    free(*s);

    //clean up dangling point
    *s = NULL;
}

//purpose: adds an element to the stack
//return: n/a
void stack_push(stack* s, void* element) {
    node* new_node = (node*)malloc(sizeof(node));

    //set up node
    new_node->element = element;
    new_node->next = s->head;

    //attach node
    s->head = new_node;
    s->size++;
}

//purpose: create a new stack and return it
//return: a pointer to an element that has been removed
void* stack_pop(stack* s) {
    if (s->size == 0) {
        printf("Error: Tried to pop from empty stack.");
        exit(1);
    }

    node* active_node = s->head;
    void* data = active_node->element;

    //update stack
    s->head = s->head->next;
    s->size--;

    free(active_node);
    return data;
}


//purpose: return true if stack if empty
//return: boolean to indicate if empty
int stack_is_empty(stack* s) {
    return s->size == 0;
}


//purpose: returns num of elements in stack
//return: total num of elements
int stack_size(stack* s) {
    return s->size;
}


//purpose: display contents of the stack as integers starting at the top
//return: n/a
void stack_display(stack* s) {
    printf("top");

    node* iter = s->head;

    while(iter != NULL) {
        printf("%d\n", (int)iter->element);
        iter = iter->next;
    }
}




