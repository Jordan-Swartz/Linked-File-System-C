/**
 * @file memory_management.h
 * @brief Defines the methods for memory management in the file system.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025
 */

#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H
#include <stack.h>

/*Function Declarations*/
/**
 * Frees memory for an array of strings.
 *
 * @param path The array of strings.
 */
void free_path(char** path);

/**
 * 
 * @param stack 
 */
void free_stack(Stack* stack);

#endif //MEMORY_MANAGEMENT_H
