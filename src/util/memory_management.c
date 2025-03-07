/**
 * memory management operations:
 *
 * allocate memory block when a file grows (sim disk space on file)
 * deallocate memory when file is deleted
 * handle fragmentation
 * track total memory management (usage, disk space)
 * delete file
 *
 *
 * create and delete memory for nodes
 */

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

        // Free only dynamically allocated strings
        if (item->type == STACK_STRING) {
            free(item->data);
        }
    }
}