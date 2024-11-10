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
    //create and initialize system
    FileSystem system;
    system_load(&system);

    //set current to system root
    FSNode* current = system.root;
    display_directory_nodes(&system, current);

    //test
    create_node(&system, current);
    create_node(&system, current);
    create_node(&system, current);

    display_current_path(&system, current);
    display_directory_nodes(&system, current);


    //collect input command
    char input;

    do {

    } while (input != 'q');

    system_save();
    return 0;
}

/**
 *
 */
void system_load(FileSystem* system) {
    //open system file to read
    FILE* file = fopen("", "r");

    if (file == NULL) {
        system_setup(system);
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
void collect_input_command() {

}

/**
 *
 */
void display_current_path(const FileSystem* system, FSNode* current) {
    char path[1024] = "$";
    char temp[100] = "";
    FSNode* iter = current;

    //create path starting from current and traversing up to the root
    while (iter != NULL) {
        sprintf(temp,"/%s%s", iter->name, path);
        strcpy(path, temp);
        iter = iter->parent;
    }

    //append system name to front of path
    sprintf(temp, "%s~%s", system->host_signature, path);
    strcpy(path, temp);

    //display path
    printf("%s\n", path);
    fflush(stdout);
}

/**
 *
 */
void display_directory_nodes(const FileSystem* system, FSNode* current) {
    FSNode* iter = current->child_head;

    //display size
    printf("size: %d\n", current->size);

    while (iter != NULL) {
        printf("%s_%s %d %s %s\n", NodeTypeNames[iter->type], PermissionsNames[iter->permissions],
            iter->size, system->hostname, iter->name);
        iter = iter->next;
    }
}






