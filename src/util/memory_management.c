#include <stdlib.h>
#include "memory_management.h"

void free_path(char** path) {
    int i = 0;
    while (path[i] != NULL) {
        free(path[i]);
        i++;
    }
    free(path);
}

void free_stack(Stack* stack) {
    while (!is_empty(stack)) {
        StackItem* item = (StackItem*)pop(stack);

        //free dynamically allocated strings
        if (item->type == STACK_STRING) {
            free(item->data);
        }
    }
}