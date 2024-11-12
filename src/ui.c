/**
 * user interface:
 *
 * prompt for commands
 * display outputs
 *
 */

#include <stdio.h>		//I/O features
#include <string.h>		//String lib

//header files
#include "ui.h"
#include "file_system.h"
#include "file_operation.h"


int main(void) {
    //load menu
    load_menu();

    //create and initialize system
    FileSystem system;
    system_load(&system);

    //set current to system root
    FSNode* current = system.root;

    //clear buffer before processing input
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    //process input
    do {
        display_menu();
    } while (process_input_command(&system, current) != Exit);

    //save system state, free memory and end program
    system_save(&system);
    free_menu();
    return 0;
}

/**
 *
 */
void display_menu() {
    if (menu_content != NULL) {
        printf("\n%s\n", menu_content);
    } else {
        printf("Error: menu content not loaded\n");
    }
}

/**
 *
 */
int process_input_command(const FileSystem* system, FSNode* current) {
    //commands list:

    //display current path
    display_current_path(system, current);

    /////collect input
    char input_str[1024];
    fgets(input_str, sizeof(input_str), stdin);
    input_str[strcspn(input_str, "\n")] = 0;

    //parse command and arg
    char* command = strtok(input_str, " ");
    char* argument = strtok(NULL, " ");

    //process make directory
    if (strcmp(command, "mkdir") == 0) {
        if (argument != NULL) {
            create_node(system, current, argument, Directory);
            return Success;

        } else {
            printf("\nError: '%s' missing argument\n", command);
            return Error;
        }

    }
    //process display current path
    else if (strcmp(command, "pwd") == 0) {
        display_current_path(system, current);
        printf("\n");
        return Success;

    }
    //process display directory contents
    else if (strcmp(command, "ls") == 0)  {
        if (argument != NULL) {
            printf("bop :P");
        } else {
            display_directory_nodes(system, current);
            // printf("\n");
        }
        return Success;

    }
    //process exit system
    else if (strcmp(command, "exit") == 0)  {
        return Exit;

    }
    //process unknown command
    else {
        printf("\nError: '%s' command not found\n", command);
        return Error;
    }
}








