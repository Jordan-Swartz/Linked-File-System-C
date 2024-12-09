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

#include <stdlib.h>

#include "file_system.h"
#include "file_operation.h"


int main(int argc, char* argv) {
    //load menu
    load_menu();

    //create and initialize system
    FileSystem system;

    // if (argc == 2) {
    //     char* existing_system = (char*)argv[1];
    //     system_load(&system, existing_system);
    // } else {
    //     printf("Error: missing file for system\n");
    //     return Error;
    // }

    //TEST
    char* existing_system = "../data/test.txt";
    system_load(&system, existing_system);

    //set current to system root
    FSNode* current = system.root;

    //clear buffer before processing input
    int ch;
    while ((ch = getchar()) != '\n' && ch != Error);
    display_menu();

    //process input
    do {
        //display_menu();
    } while (process_input_command(&system, &current) != Exit);

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
int process_input_command(const FileSystem* system, FSNode** current) {
    //display current path
    display_current_path(system, (*current));

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
    //FIXME



    //handle case if arg has ' / '




        if (argument != NULL) {
            create_node(system, (*current), argument, Directory);
            return Success;

        } else {
            printf("\nError: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process make file
    else if (strcmp(command, "touch") == 0) {
        if (argument != NULL) {
            create_node(system, (*current), argument, File);
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
            while (strcmp((*current)->name, system->root->name) != 0) {
                change_directory_backward(current);
            }
            return Success;
        }

        //process change to previous '..'
        if (strcmp(argument, "..") == 0) {
            if (strcmp((*current)->name, system->root->name) == 0) {
                printf("Error: Already at the root directory.\n");
                return Error;
            }
            change_directory_backward(current);
            return Success;
        }

        // //parse and process forward change
        // char temp[256] = {0};
        // int i = 0, temp_index = 0, change_result = -1;

        //parse argument
        char** parsed_path = parse_relative_path(argument);
        int i = 0, change_result = -1;;

        while (parsed_path[i] != NULL) {
            change_result = change_directory_forward(current, parsed_path[i]);
            if (change_result == Error_File) {
                printf("Error: '%s' is not a directory.\n", parsed_path[i]);
                free_relative_path(parsed_path);
                return Error;
            } else if (change_result == Error) {
                printf("\nError: '%s' -> No such file or directory\n", parsed_path[i]);
                free_relative_path(parsed_path);
                return Error;
            } else {
                i++;
            }
        }
        free_relative_path(parsed_path);
        return Success;


        // while (argument[i] != '\0') {
        //     if (argument[i] == '/') {
        //         //change current to directory stored in buffer
        //         change_result = change_directory_forward(current, temp);
        //         if (change_result == Success) {
        //             temp[0] = '\0';
        //             temp_index = 0;
        //         } else if (change_result == Error_File) {
        //             printf("Error: '%s' is not a directory.\n", temp);
        //             return Error;
        //         } else {
        //             printf("\nError: '%s' -> No such file or directory\n", temp);
        //             return Error;
        //         }
        //     } else {
        //         //append char to buffer
        //         temp[temp_index] = argument[i];
        //         temp_index++;
        //     }
        //     i++;
        // }
        //
        // //process single directory change
        // change_result = change_directory_forward(current, temp);
        // if (change_result == Error_File) {
        //     printf("Error: '%s' is not a directory.\n", temp);
        //     return Error;
        // } else if (change_result == Error) {
        //     printf("\nError: '%s' -> No such file or directory\n", temp);
        //     return Error;
        // }

    }

    //process display directory contents
    else if (strcmp(command, "ls") == 0)  {
        if (argument != NULL) {
            //parse arg to change to directory and then display its contents
            //TODO finish method
            //refactor parsing in change directory method to helper

        } else {
            display_directory_nodes(system, (*current));
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

//return array of parsed strings
char** parse_relative_path(const char* argument) {
    char temp[256];
    int i = 0, temp_index = 0, arr_index = 0;

    //array of strings
    char** parsed_path = (char**)malloc(sizeof(char*) * 256);

    while (argument[i] != '\0') {
        if (argument[i] == '/') {
            //add null char
            temp[temp_index] = '\0';

            //allocate memory for and copy temp to substring
            char* substring = (char*)malloc(temp_index + 1);
            strcpy(substring, temp);

            //add substring to array, reset buffer index, and clear buffer
            parsed_path[arr_index] = substring;
            arr_index++;
            temp_index = 0;
            memset(temp, 0, sizeof(temp));
        } else {
            //add next char to buffer
            temp[temp_index] = argument[i];
            temp_index++;
        }
        i++;
    }

    //handle final substring if needed
    if (temp_index > 0) {
        temp[temp_index] = '\0';
        char* substring = (char*)malloc(temp_index + 1);
        strcpy(substring, temp);
        parsed_path[arr_index] = substring;
        arr_index++;
    }

    //null terminate array
    parsed_path[arr_index] = NULL;

    return parsed_path;
}

void free_relative_path(char** path) {
    int i = 0;
    while (path[i] != NULL) {
        free(path[i]);
        i++;
    }
    free(path);
}








