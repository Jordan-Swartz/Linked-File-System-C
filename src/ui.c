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

#include <stdbool.h>
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
    while ((ch = getchar()) != '\n' && ch != Error)
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
    fgets(input_str, sizeof(input_str), stdin);           //get entire line
    input_str[strcspn(input_str, "\n")] = 0;        //remove newline char

    //parse command and arg
    char* command = strtok(input_str, " ");
    char* argument = strtok(NULL, " ");
    char* argument2 = strtok(NULL, " ");

    //process make directory
    if (strcmp(command, "mkdir") == 0) {
        process_mkdir(system, command, argument, argument2, (*current));
    }

    //process make file
    else if (strcmp(command, "touch") == 0) {
        //check for argument errors
        if (argument2 != NULL) {
            printf("Error: '%s' too many arguments\n", command);
            return Error;
        } else if (argument == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }

        //FIXME Support abs/rel paths

        //ensure touch arg is not an invalid name
        if (strcmp(argument, "~") == 0 || strcmp(argument, "/") == 0
            || strcmp(argument, ".") == 0 || strcmp(argument, "..") == 0) {
            printf("Error: Cannot create file named '%s'.\n", argument);
            return Error;
        }

        //make file node
        create_node(system, (*current), argument, File);
        return Success;
    }

    //process delete file or directory
    else if (strcmp(command, "rm") == 0) {
        if (argument != NULL) {
            //TODO finish method

            //FIXME
            //support ../ implementation

        } else {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }

    //process display current path
    else if (strcmp(command, "pwd") == 0) {
        process_pwd((*current));
        return Success;
    }

    //process changing directory
    else if (strcmp(command, "cd") == 0) {
        process_cd(system, command, argument, argument2, current);
    }

    //process display directory contents
    else if (strcmp(command, "ls") == 0)  {
        process_ls(system, command, argument, argument2, (*current));
    }

    //process move file or directory
    else if (strcmp(command, "mv") == 0) {
        //check for argument errors
        if (argument == NULL || argument2 == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }

        //FIXME
        /*
        * jordan@JDS:~/a/b/c$ mv /a/b /
            Error: Invalid or empty path.
            jordan@JDS:~/a/b/c$ mv /a/b ~
        * */

        //ensure desired source is not root directory
        if (strcmp(argument, "~") == 0 || strcmp(argument, "/") == 0) {
            printf("Error: Cannot move the root directory.\n");
            return Error;
        }

        //traverse to second-to-last parsed path to find parent of source node
        FSNode* source_node_parent  = NULL;
        char** parsed_path_source = parse_path(argument);
        int i = 0, change_result = -1;

        //ensure path is not null
        if (parsed_path_source[0] == NULL) {
            printf("Error: Invalid or empty path.\n");
            free_path(parsed_path_source);
            return Error;
        }

        //relative or abs path
        if (argument[0] == '/') {
            source_node_parent = system->root;
        } else {
            source_node_parent = (*current);
        }

        while (parsed_path_source[i+1] != NULL) {
            //process backwards change
            if (strcmp(parsed_path_source[i], "..") == 0) {
                if (strcmp(source_node_parent->name, system->root->name) == 0) {
                    printf("Error: Cannot move past the root directory.\n");
                    free_path(parsed_path_source);
                    return Error;
                }
                change_directory_backward(&source_node_parent);

            } else {
                //process forward change
                change_result = change_directory_forward(&source_node_parent, parsed_path_source[i]);

                if (change_result != Success) {
                    printf("Error: Cannot access '%s' -> No such file or directory.\n", parsed_path_source[i]);
                    free_path(parsed_path_source);
                    return Error;
                }
            }
            //move to next string
            i++;
        }

        //search for node to move within source_node_parent directory
        FSNode* source_node = find_node(source_node_parent, parsed_path_source[i]);
        if (source_node == NULL) {
            printf("Error: Cannot access '%s' -> No such file or directory.\n", parsed_path_source[i]);
            free_path(parsed_path_source);
            return Error;
        }

        //traverse to second-to-last parsed path to find destination node
        FSNode* destination_node = NULL;
        char** parsed_path_destination = parse_path(argument2);
        i = 0, change_result = -1;

        //ensure path is not null
        if (parsed_path_destination[0] == NULL) {
            printf("Error: Invalid or empty path.\n");
            free_path(parsed_path_destination);
            return Error;
        }

        //relative or abs path
        if (argument2[0] == '/') {
            destination_node = system->root;
            //i++;
        } else {
            destination_node = (*current);
        }

        //if destination is not root then parse path and verify destination
        if (strcmp(argument2, "~") != 0 && strcmp(argument2, "/") != 0) {
            while (parsed_path_destination[i] != NULL) {
                //process backwards change
                if (strcmp(parsed_path_destination[i], "..") == 0) {
                    if (strcmp(destination_node->name, system->root->name) == 0) {
                        printf("Error: Cannot move past the root directory.\n");
                        free_path(parsed_path_destination);
                        return Error;
                    }
                    change_directory_backward(&destination_node);

                } else {
                    //process forward change
                    change_result = change_directory_forward(&destination_node, parsed_path_destination[i]);

                    if (change_result != Success) {
                        printf("Error: Cannot access '%s' -> No such file or directory.\n", parsed_path_destination[i]);
                        free_path(parsed_path_destination);
                        return Error;
                    }
                }
                //move to next string
                i++;
            }

            //ensure destination is not the source node
            if (source_node == destination_node) {
                printf("Error: Cannot move '%s' -> Destination is the same as the source.\n", source_node->name);
                free_path(parsed_path_source);
                free_path(parsed_path_destination);
                return Error;
            }

            //ensure destination is not a child of the source node
            if (is_subdirectory(source_node, destination_node) != Success) {
                printf("Error: Cannot move '%s' -> Destination is a subdirectory the source.\n", source_node->name);
                free_path(parsed_path_source);
                free_path(parsed_path_destination);
                return Error;
            }
        }

        //ensure destination node is valid (no node with existing name)
        if (find_node(destination_node, source_node->name) != NULL) {
            printf("Error: Cannot move '%s' -> A node with this name already exists in the destination directory.\n",
                source_node->name);
            free_path(parsed_path_source);
            free_path(parsed_path_destination);
            return Error;
        }

        //remove source node from current directory
        if (source_node == source_node_parent->child_head) {
            source_node_parent->child_head = source_node->next;
        }

        //if target is not head
        if (source_node->previous != NULL) {
            source_node->previous->next = source_node->next;
        }

        //if target is not last
        if (source_node->next != NULL) {
            source_node->next->previous = source_node->previous;
        }

        source_node->previous = NULL;
        source_node->next = NULL;
        source_node_parent->size--;

        //move source node into destination node
        insert_node(destination_node, source_node);

        //update parent
        source_node->parent = destination_node;

        free_path(parsed_path_source);
        free_path(parsed_path_destination);
        return Success;
    }

    //process rename file or directory
    else if (strcmp(command, "rn") == 0) {
        //check for argument errors
        if (argument == NULL || argument2 == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }

        //parse second-to-last for rename source
        FSNode* current_copy = NULL;
        FSNode* rename_node = NULL;
        char** parsed_path = parse_path(argument);
        int i = 0, change_result = -1;

        //ensure path is not null
        if (parsed_path[0] == NULL) {
            printf("Error: Invalid or empty path.\n");
            free_path(parsed_path);
            return Error;
        }

        //relative or abs path
        if (argument[0] == '/') {
            current_copy = system->root;
        } else {
            current_copy = (*current);
        }

        //if rename node is not root then parse path to find desired node
        if (strcmp(argument, "~") != 0 && strcmp(argument, "/") != 0) {
            while (parsed_path[i + 1] != NULL) {
                //process backwards change
                if (strcmp(parsed_path[i], "..") == 0) {
                    if (strcmp(current_copy->name, system->root->name) == 0) {
                        printf("Error: Cannot move past the root directory.\n");
                        free_path(parsed_path);
                        return Error;
                    }
                    change_directory_backward(&current_copy);

                } else {
                    //process forward change
                    change_result = change_directory_forward(&current_copy, parsed_path[i]);

                    if (change_result != Success) {
                        printf("Error: Cannot access '%s' -> No such file or directory.\n", parsed_path[i]);
                        free_path(parsed_path);
                        return Error;
                    }
                }
                //move to next string
                i++;
            }

            //search for node to rename in current directory
            rename_node = find_node(current_copy, parsed_path[i]);
            if (rename_node == NULL) {
                printf("Error: Cannot rename '%s' -> No such file or directory.\n", parsed_path[i]);
                free_path(parsed_path);
                return Error;
            }
        } else {
            rename_node = system->root;
        }

        //parse argument2 for safety and rename based on first index
        char** parsed_path_rename = parse_path(argument2);
        if (parsed_path_rename[0] == NULL) {
            printf("Error: Invalid or empty renaming path.\n");
            free_path(parsed_path);
            free_path(parsed_path_rename);
            return Error;
        }

        //reject invalid naming
        if (strcmp(parsed_path_rename[0], ".") == 0 || strcmp(parsed_path_rename[0], "..") == 0
            || strcmp(parsed_path_rename[0], "/") == 0 || strcmp(parsed_path_rename[0], "~") == 0) {
            printf("Error: Cannot rename directory named '%s'.\n", parsed_path_rename[0]);
            free_path(parsed_path_rename);
            free_path(parsed_path);
            return Error;
            }

        //rename node and free path
        strcpy(rename_node->name, parsed_path_rename[0]);
        free_path(parsed_path_rename);
        free_path(parsed_path);
        return Success;
    }

    //process changing permissions for file
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

    //process displaying menu
    else if (strcmp(command, "menu") == 0) {
        display_menu();
        return Success;
    }

    //PROCESS TEST
    else if (strcmp(command, "p") == 0) {
        char** parsed_path = parse_path(argument);
        int i = 0;
        printf("Path: ");
        while (parsed_path[i] != NULL) {
            printf("%s, ", parsed_path[i]);
            i++;
        }
    }

    //PROCESS TEST Parent
    else if (strcmp(command, "p2") == 0) {
        FSNode* iter = (*current)->child_head;
        while (iter != NULL) {
            printf("current: %s parent: %s\n", iter->name, iter->parent->name);
            iter = iter->next;
        }
    }

    //process exit system
    else if (strcmp(command, "exit") == 0)  {
        printf("Exiting System...\n");
        return Exit;
    }

    //process unknown command
    else {
        printf("Error: '%s' command not found\n", command);
        return Error;
    }
}

//REFACTOR

int process_input_command_2() {
    return 0;
}

int validate_args(const char* command, const char* arg1, const char* arg2) {
    if (arg2 != NULL) {
        printf("Error: '%s' too many arguments\n", command);
        return Error;
    } else if (arg1 == NULL) {
        printf("Error: '%s' missing argument\n", command);
        return Error;
    }
    return Success;
}

int process_parsed_path(
    const FileSystem* system,
    const char* path,
    FSNode* start_node,
    FSNode** return_node,
    const int stop_at_second_last,
    const int enable_create_node
    )
{
    //parse path and ensure it is not null
    char** parsed_path = parse_path(path);
    if (parsed_path[0] == NULL) {
        printf("Error: Invalid or empty path.\n");
        free_path(parsed_path);
        return Error;
    }

    //determine relative or abs path
    FSNode* current = NULL;
    int i = 0, change_result = -1;

    if (path[0] == '/') {
        current = system->root;
    } else {
        current  = start_node;
    }

    //traverse path
    while (parsed_path[i] != NULL) {
        //break if stop at second to last is true (mv and rn)
        if (stop_at_second_last == STOP_AT_LAST && parsed_path[i+1] == NULL) {
            break;
        }

        //reject invalid naming (mkdir and rn)
        if (strcmp(parsed_path[i], ".") == 0 || strcmp(parsed_path[i], "~") == 0
            || strcmp(parsed_path[i], "/") == 0) {
            printf("Error: Cannot create or access directory named '%s'.\n", parsed_path[i]);
            free_path(parsed_path);
            return Error;
        }

        //process backwards change
        if (strcmp(parsed_path[i], "..") == 0) {
            if (strcmp(current->name, system->root->name) == 0) {
                printf("Error: Cannot move past the root directory.\n");
                free_path(parsed_path);
                return Error;
            }
            change_directory_backward(&current);

        }
        //process forward change
        else {
            //handle node creation (mkdir)
            if (enable_create_node == ENABLE_CREATE) {
                //create node
                change_result = create_node(system, current, parsed_path[i], Directory);
                if (change_result != Success) {
                    printf("Error: '%s' A node with this name already exists.\n", parsed_path[i]);
                    free_path(parsed_path);
                    return Error;
                } else if (parsed_path[i+1] == NULL) {
                    //if last node path has been created then break
                    break;
                }
            }
            //move current forward
            change_result = change_directory_forward(&current, parsed_path[i]);

            if (change_result != Success) {
                printf("Error: Cannot access '%s' -> No such file or directory.\n", parsed_path[i]);
                free_path(parsed_path);
                return Error;
            }
        }
        //move to next string
        i++;
    }

    //set target node to last traversed address
    if (return_node != NULL) {
        (*return_node) = current;
    }
    free_path(parsed_path);
    return Success;
}

void process_mkdir(
    const FileSystem* system,
    const char* command,
    const char* arg1,
    const char* arg2,
    FSNode* current
    )
{
    //check for argument errors
    if (validate_args(command, arg1, arg2) == Error) {
        return;
    }

    //handle creation of nested directories
    process_parsed_path(system, arg1, current, IGNORE_RESULT, FULL_TRAVERSAL, ENABLE_CREATE);
}

void process_touch() {

}

void process_rm() {

}

void process_pwd(FSNode* current) {
    FSNode* iter = current;
    char path[1024] = "";
    char temp[100];

    //create path
    while (iter != NULL) {
        sprintf(temp, "/%s%s", iter->name, path);
        strcpy(path, temp);
        iter = iter->parent;
    }

    printf("%s\n", path);
}

void process_cd(
    const FileSystem* system,
    const char* command,
    const char* arg1,
    const char* arg2,
    FSNode** current
    )
{
    //check for argument errors
    if (validate_args(command, arg1, arg2) == Error) {
        return;
    }

    //process change to root '~' or '/'
    if (strcmp(arg1, "~") == 0 || strcmp(arg1, "/") == 0) {
        (*current) = system->root;
        return;
    }

    //handle traversal of directories
    process_parsed_path(system, arg1, (*current), current, FULL_TRAVERSAL, DISABLE_CREATE);
}

void process_ls(
    const FileSystem* system,
    const char* command,
    const char* arg1,
    const char* arg2,
    FSNode* current
    )
{
    //check for argument errors
    if (validate_args(command, "arg1", arg2) == Error) {
        return;
    }

    //process ls by itself
    if (arg1 == NULL) {
        display_directory_nodes(system, current);
        return;
    }

    //process ls for root '~' or '/'
    if (strcmp(arg1, "~") == 0 || strcmp(arg1, "/") == 0) {
        display_directory_nodes(system, system->root);
        return;
    }

    //handle traversal of directories before ls
    FSNode* return_node = current;
    if (process_parsed_path(system, arg1, current, &return_node, FULL_TRAVERSAL, DISABLE_CREATE) != Error) {
        display_directory_nodes(system, return_node);
    }
}

void process_mv(
    const FileSystem* system,
    const char* command,
    const char* arg1,
    const char* arg2,
    FSNode* current
    )
{
    //check for argument errors
    if (validate_args(command, "arg1", arg2) == Error) {
        return;
    }

    //process ls by itself
    if (arg1 == NULL) {
        display_directory_nodes(system, current);
        return;
    }

    //process ls for root '~' or '/'
    if (strcmp(arg1, "~") == 0 || strcmp(arg1, "/") == 0) {
        display_directory_nodes(system, system->root);
        return;
    }

    //handle traversal of directories before ls
    FSNode* return_node = current;
    if (process_parsed_path(system, arg1, current, &return_node, FULL_TRAVERSAL, DISABLE_CREATE) != Error) {
        display_directory_nodes(system, return_node);
    }
}








