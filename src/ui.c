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


int main(int argc, char* argv) {
    //load menu
    load_menu();

    //create and initialize system
    FileSystem system;

    if (argc == 2) {
        char* existing_system = (char*)argv[1];
        system_load(&system, existing_system);
    } else {
        printf("Error: missing file for system\n");
        return Error;
    }

    //set current to system root
    FSNode* current = system.root;

    //clear buffer before processing input
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    display_menu();

    //process input
    do {
        //display_menu();
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
    //display current path
    display_current_path(system, current);

    /////collect input
    char input_str[1024];
    fgets(input_str, sizeof(input_str), stdin);      //get entire line
    input_str[strcspn(input_str, "\n")] = 0;    //remove newline char

    //parse command and arg
    char* command = strtok(input_str, " ");
    char* argument = strtok(NULL, " ");
    char* argument2 = strtok(NULL, " ");

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

    //process make file
    else if (strcmp(command, "touch") == 0) {
        if (argument != NULL) {
            create_node(system, current, argument, File);
            return Success;

        } else {
            printf("\nError: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process delete file or directory
    else if (strcmp(command, "rm") == 0) {
        if (argument != NULL) {
            //TODO finish method

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

    //process changing directory
    else if (strcmp(command, "cd") == 0) {
        //check for argument errors
        if (argument2 != NULL) {
            printf("\nError: '%s' too many arguments\n", command);
            return Error;
        } else if (argument == NULL) {
            printf("\nError: '%s' missing argument\n", command);
            return Error;
        }

        //process change to root '~'
        if (argument[0] == '~') {
            while (strcmp(current->name, system->root->name) != 0) {
                change_directory_backward(&current);
            }
            return Success;
        }

        //process change to previous '..'
        if (strcmp(argument, "..") == 0) {
            if (strcmp(current->name, system->root->name) == 0) {
                printf("Error: Already at the root directory.");
                return Error;
            }
            change_directory_backward(&current);
            return Success;
        }

        //parse and process forward change
        char temp[256];
        int i = 0;
        int temp_index = 0;

        while (argument[i] != '\0') {
            if (argument[i] == '/') {
                //TODO finish method
                //process temp
                //if temp process is success:
                //skip to next char, reset temp and index;
                if (change_directory_forward(current, temp) != Error) {
                    temp[0] = '\0';
                    temp_index = 0;
                } else {
                    printf("\nError: No such file or directory\n");
                    return Error;
                }
            } else {
                //append char to buffer
                temp[temp_index] = argument[i];
                temp_index++;
            }
            i++;
        }
    }

    //process display directory contents
    else if (strcmp(command, "ls") == 0)  {
        if (argument != NULL) {
            //parse arg to change to directory and then display its contents
            //TODO finish method

        } else {
            display_directory_nodes(system, current);
            // printf("\n");
        }
        return Success;
    }

    //process move file or directory
    else if (strcmp(command, "mv") == 0) {
        if (argument != NULL || argument2 != NULL) {
            //TODO finish method

        } else {
            printf("\nError: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process rename file or directory
    else if (strcmp(command, "mv") == 0) {
        if (argument != NULL || argument2 != NULL) {
            //TODO finish method

        } else {
            printf("\nError: '%s' missing argument\n", command);
            return Error;
        }
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

char* parse_relative_path() {

}









