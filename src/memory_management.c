/**
 * memory management operations:
 *
 * allocate memory block when a file grows (sim disk space on file)
 * deallocate memory when file is deleted
 * handle framentation
 * track total memory management (usage, disk space)
 * delete file
 *
 *
 * create and deleete memory for nodes
 */

#include <stdlib.h>
#include "memory_management.h"

//free array of strings
void free_path(char** path) {
    int i = 0;
    while (path[i] != NULL) {
        free(path[i]);
        i++;
    }
    free(path);
}