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

    //test
    create_node(current);
    create_node(current);
    create_node(current);

    display_current_path(&system, current);

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
void display_current_path(const FileSystem* system, FSNode* current) {
    FSNode* iter = current;

    //path hostname
    char path[1024] = "";
    strcpy(path, system->hostname);
    strcat(path, "~$");

    //create path starting at root
    while (iter != NULL) {
        strcat(path, "/");
        strcat(path, iter->name);
        iter = iter->next;
    }

    //display path
    strcat(path, "$");
    printf("%s", path);

    //FIX ME: GO FROM CURRENT TO ROOT NOT REVERSE
}






