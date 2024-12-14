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
    fgets(input_str, sizeof(input_str), stdin);     //get entire line
    input_str[strcspn(input_str, "\n")] = 0;        //remove newline char

    //parse command and arg
    char* command = strtok(input_str, " ");
    char* argument = strtok(NULL, " ");
    char* argument2 = strtok(NULL, " ");

    //process make directory
    if (strcmp(command, "mkdir") == 0) {
        if (argument == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }

        //create nested directories per input 'mkdir d1/d2/d3'
        char** parsed_path = parse_path(argument);
        FSNode* original_current = (*current);
        int i = 0, result = Success;

        while (parsed_path[i] != NULL && result == Success) {
            //attempt to create directory
            result = create_node(system, (*current), parsed_path[i], Directory);

            if (result == Error) {
                printf("Error: A node with this name already exists.\n");
                free_path(parsed_path);
                return Error;
            } else if (parsed_path[i+1] != NULL) {
                //change to new directory
                change_directory_forward(current, parsed_path[i]);
            }
            i++;
        }

        //free parsed path and revert current back to original
        (*current) = original_current;
        free_path(parsed_path);
        return Success;
    }

    //process make file
    else if (strcmp(command, "touch") == 0) {
        if (argument != NULL) {
            create_node(system, (*current), argument, File);
            return Success;

        } else {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process delete file or directory
    else if (strcmp(command, "rm") == 0) {
        if (argument != NULL) {
            //TODO finish method

        } else {
            printf("Error: '%s' missing argument\n", command);
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
            printf("Error: '%s' too many arguments\n", command);
            return Error;
        } else if (argument == NULL) {
            printf("Error: '%s' missing argument\n", command);
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

        //parse argument
        char** parsed_path = parse_path(argument);
        int i = 0, change_result = -1;

        while (parsed_path[i] != NULL) {
            change_result = change_directory_forward(current, parsed_path[i]);
            if (change_result == Error_File) {
                printf("Error: '%s' is not a directory.\n", parsed_path[i]);
                free_path(parsed_path);
                return Error;
            } else if (change_result == Error) {
                printf("Error: '%s' -> No such file or directory\n", parsed_path[i]);
                free_path(parsed_path);
                return Error;
            } else {
                i++;
            }
        }
        //free path array
        free_path(parsed_path);
        return Success;
    }

    //process display directory contents
    else if (strcmp(command, "ls") == 0)  {
        //check for argument errors
        if (argument2 != NULL) {
            printf("Error: '%s' too many arguments\n", command);
            return Error;

        } else if (argument == NULL) {
            //process ls by itself
            display_directory_nodes(system, (*current));
            return Success;

        } else {
            //process ls after directory change
            FSNode* original_current = (*current);
            char** parsed_path = parse_path(argument);
            int i = 0, change_result = -1;;

            while (parsed_path[i] != NULL) {
                change_result = change_directory_forward(current, parsed_path[i]);
                if (change_result != Success) {
                    printf("Error: cannot access '%s' -> No such file or directory.\n", parsed_path[i]);
                    free_path(parsed_path);
                    return Error;
                }
                i++;
            }

            //display directory contents and then revert current back to original directory
            display_directory_nodes(system, (*current));
            free_path(parsed_path);
            (*current) = original_current;
        }
        return Success;
    }

    //process move file or directory
    else if (strcmp(command, "mv") == 0) {
        if (argument != NULL || argument2 != NULL) {
            //TODO finish method

        } else {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process rename file or directory
    else if (strcmp(command, "rn") == 0) {
        //check for argument errors
        if (argument == NULL || argument2 == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }



        //FIXME




        //traverse to second-to-last node in parsed path
        FSNode* original_current = (*current);
        char** parsed_path = parse_path(argument);
        int i = 0, change_result = -1;;

        while (parsed_path[i + 1] != NULL) {
            change_result = change_directory_forward(current, parsed_path[i]);
            if (change_result != Success) {
                printf("Error: cannot access '%s' -> No such file or directory.\n", parsed_path[i]);
                free_path(parsed_path);
                return Error;
            }
            i++;
        }

        //search for node to rename in current directory
        FSNode* rename_node = find_node(current, parsed_path[i]);
        if (rename_node == NULL) {
            printf("Error: cannot rename '%s' -> No such file or directory.\n", parsed_path[i]);
            free_path(parsed_path);
            return Error;
        } else {
            strcpy(rename_node->name, argument2);
        }

        free_path(parsed_path);
        (*current) = original_current;
        return Success;
    }

    //process changing permissons for file
    else if (strcmp(command, "chmod") == 0) {
        if (argument != NULL || argument2 != NULL) {
            //TODO finish method

        } else {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process copying file or directory into a new destination
    else if (strcmp(command, "cp") == 0) {
        if (argument != NULL || argument2 != NULL) {
            //TODO finish method
            //recursive algorithm that recreates all the contents

        } else {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process finding file or directory in current directory
    else if (strcmp(command, "find") == 0) {
        if (argument != NULL || argument2 != NULL) {
            //TODO finish method
            //recursive algorithm that recreates all the contents

        } else {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process displaying command history for the current session
    else if (strcmp(command, "history") == 0) {
        if (argument != NULL || argument2 != NULL) {
            //TODO finish method
            //recursive algorithm that recreates all the contents

        } else {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process cat
    else if (strcmp(command, "cat") == 0) {
        if (argument != NULL || argument2 != NULL) {
            //TODO finish method
            //recursive algorithm that recreates all the contents

        } else {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process exit system
    else if (strcmp(command, "exit") == 0)  {
        return Exit;
    }

    //process unknown command
    else {
        printf("Error: '%s' command not found\n", command);
        return Error;
    }
}











