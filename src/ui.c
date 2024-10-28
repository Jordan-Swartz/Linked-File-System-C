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




