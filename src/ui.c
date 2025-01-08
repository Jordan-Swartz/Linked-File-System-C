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
        return Success;
    }

    //process make file
    else if (strcmp(command, "touch") == 0) {
        process_touch(system, command, argument, argument2, (*current));
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
        return Success;
    }

    //process display directory contents
    else if (strcmp(command, "ls") == 0)  {
        process_ls(system, command, argument, argument2, (*current));
        return Success;
    }

    //process move file or directory
    else if (strcmp(command, "mv") == 0) {
        process_mv(system, command, argument, argument2, (*current));
        return Success;
    }

    //process rename file or directory
    else if (strcmp(command, "rn") == 0) {
        process_rn(system, command, argument, argument2, (*current));
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

int validate_args(const char* command, const char* arg1, const char* arg2, const int arg_condition) {
    //single argument check
    if (arg_condition == SINGLE_ARG) {
        if (arg2 != NULL) {
            printf("Error: '%s' too many arguments\n", command);
            return Error;
        } else if (arg1 == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }
    //double argument check
    else if (arg_condition == DOUBLE_ARG) {
        if (arg1 == NULL || arg2 == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }
    }
    return Success;
}

int process_parsed_path(
    const FileSystem* system,
    const char* path,
    FSNode* start_node,
    FSNode** return_node,
    char** return_name,
    const int stop_at_second_last,
    const int enable_create_node,
    const int enable_name_node
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

        //reject invalid naming (mkdir and touch)
        if (enable_name_node == ENABLE_NAME) {
            if (strcmp(parsed_path[i], ".") == 0 || strcmp(parsed_path[i], "~") == 0
                || strcmp(parsed_path[i], "/") == 0 || strcmp(parsed_path[i], "..") == 0) {
                printf("Error: Cannot create or access node named '%s'.\n", parsed_path[i]);
                free_path(parsed_path);
                return Error;
                }
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
        //process root change
        else if (strcmp(parsed_path[i], "/") == 0 || strcmp(parsed_path[i], "~") == 0) {
            current = system->root;
            if (return_name != NULL) {
                (*return_name) = strdup(parsed_path[i]);
            }
            break;
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
                printf("Error: Cannot access file or directory '%s'.\n", parsed_path[i]);
                free_path(parsed_path);
                return Error;
            }
        }
        //move to next string
        i++;
    }

    //set return node to last traversed address
    if (return_node != NULL) {
        (*return_node) = current;
    }

    //set return name to last parsed path
    if (parsed_path[i] != NULL && return_name != NULL) {
        (*return_name) = strdup(parsed_path[i]);
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
    if (validate_args(command, arg1, arg2, SINGLE_ARG) == Error) {
        return;
    }

    //handle creation of nested directories with full traversal
    process_parsed_path(system, arg1, current, IGNORE_RESULT, NULL,
        FULL_TRAVERSAL, ENABLE_CREATE, ENABLE_NAME);
}

void process_touch(
    const FileSystem* system,
    const char* command,
    const char* arg1,
    const char* arg2,
    FSNode* current
    )
{
    //check for argument errors
    if (validate_args(command, arg1, arg2, DOUBLE_ARG) == Error) {
        return;
    }

    //traverse arg1 path to find destination for file creation
    FSNode* destination = current;
    if (process_parsed_path(system, arg1, current, &destination, NULL,
        FULL_TRAVERSAL, DISABLE_CREATE, DISABLE_NAME) == Error)
    {
        return;
    }

    //ensure touch arg is not an invalid name
    if (strcmp(arg2, "~") == 0 || strcmp(arg2, "/") == 0
        || strcmp(arg2, ".") == 0 || strcmp(arg2, "..") == 0) {
        printf("Error: Cannot create file named '%s'.\n", arg2);
        return;
    }

    //make file node
    if (create_node(system, destination, arg2, File) == Error) {
        printf("Error: '%s' A node with this name already exists.\n", arg2);
    }
}

void process_rm() {
    // recuriuvs delete

    //validate args
    //provide continue warning
    //validate path and that selected directory exists
    /**
     * start at target and for each child node if its a directory call delete on it
     * if its a file delete it
     * after target is empty delete target
     * update parent directory
     */
}

void recursive_delete() {

}

void process_pwd(FSNode* current) {
    FSNode* iter = current;
    char path[1024] = "";

    //create path
    while (iter != NULL) {
        char temp[100];
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
    if (validate_args(command, arg1, arg2, SINGLE_ARG) == Error) {
        return;
    }

    //handle full traversal of directories
    process_parsed_path(system, arg1, (*current), current, NULL,
        FULL_TRAVERSAL, DISABLE_CREATE, DISABLE_NAME);
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
    if (validate_args(command, "arg1", arg2, SINGLE_ARG) == Error) {
        return;
    }

    //process ls by itself
    if (arg1 == NULL) {
        display_directory_nodes(system, current);
        return;
    }

    //handle traversal of directories before ls
    FSNode* return_node = current;
    if (process_parsed_path(system, arg1, current, &return_node, NULL,
        FULL_TRAVERSAL, DISABLE_CREATE, DISABLE_NAME) != Error)
    {
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
    if (validate_args(command, arg1, arg2, DOUBLE_ARG) == Error) {
        return;
    }

    //ensure desired source is not root directory
    if (strcmp(arg1, "~") == 0 || strcmp(arg1, "/") == 0) {
        printf("Error: Cannot move the root directory.\n");
        return;
    }

    //traverse to last node to find parent of source node
    FSNode* source_node_parent = current;
    char* return_name_source = "";
    if (process_parsed_path(system, arg1, current, &source_node_parent, &return_name_source,
        STOP_AT_LAST, DISABLE_CREATE, DISABLE_NAME) == Error)
    {
        free(return_name_source);
        return;
    }

    //search for node to move within source_node_parent directory
    FSNode* source_node = find_node(source_node_parent, return_name_source);
    if (source_node == NULL) {
        printf("Error: Cannot access '%s' -> No such file or directory.\n", return_name_source);
        free(return_name_source);
        return;
    }

    //handle full traversal to find destination
    FSNode* destination_node = current;
    // char* return_name_destination = "";
    if (process_parsed_path(system, arg2, current, &destination_node , NULL,
        FULL_TRAVERSAL, DISABLE_CREATE, DISABLE_NAME) == Error)
    {
        free(return_name_source);
        return;
    }

    //ensure destination is not the source node
    if (source_node == destination_node) {
        printf("Error: Cannot move '%s' -> Destination is the same as the source.\n", source_node->name);
        free(return_name_source);
        return;
    }

    //ensure destination is not a child of the source node
    if (is_subdirectory(source_node, destination_node) != Success) {
        printf("Error: Cannot move '%s' -> Destination is a subdirectory the source.\n", source_node->name);
        free(return_name_source);
        return;
    }

    //ensure destination node is valid (no node with existing name)
    if (find_node(destination_node, source_node->name) != NULL) {
        printf("Error: Cannot move '%s' -> A node with this name already exists in the destination directory.\n", source_node->name);
        free(return_name_source);
        return;
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
    free(return_name_source);
}

void process_rn(
    const FileSystem* system,
    const char* command,
    const char* arg1,
    const char* arg2,
    FSNode* current
    )
{
    //check for argument errors
    if (validate_args(command, arg1, arg2, DOUBLE_ARG) == Error) {
        return;
    }

    //traverse to last node to find parent of rename node
    FSNode* rename_node_parent = current;
    char* return_name_source = "";
    if (process_parsed_path(system, arg1, current, &rename_node_parent, &return_name_source,
    STOP_AT_LAST, DISABLE_CREATE, DISABLE_NAME) == Error)
    {
        free(return_name_source);
        return;
    }

    //find rename node in parent
    FSNode* rename_node;
    if (rename_node_parent == system->root && strcmp(return_name_source, "~") == 0
        || strcmp(return_name_source, "/") == 0)
    {
        rename_node = system->root;
    } else {
        rename_node = find_node(rename_node_parent, return_name_source);
        if (rename_node == NULL) {
            printf("Error: Cannot rename '%s' -> No such file or directory.\n", return_name_source);
            free(return_name_source);
            return;
        }
    }

    //parse arg2 for naming safety and rename based on first index
    char** parsed_path_rename = parse_path(arg2);
    if (parsed_path_rename[0] == NULL) {
        printf("Error: Invalid or empty renaming path.\n");
        free(return_name_source);
        free_path(parsed_path_rename);
        return;
    }

    //reject invalid naming
    if (strcmp(parsed_path_rename[0], ".") == 0 || strcmp(parsed_path_rename[0], "..") == 0
        || strcmp(parsed_path_rename[0], "/") == 0 || strcmp(parsed_path_rename[0], "~") == 0) {
        printf("Error: Cannot rename directory named '%s'.\n", parsed_path_rename[0]);
        free(return_name_source);
        free_path(parsed_path_rename);
        return;
    }

    //rename node and free memory
    strcpy(rename_node->name, parsed_path_rename[0]);
    free(return_name_source);
    free_path(parsed_path_rename);
}

void process_chmod(
    const FileSystem* system,
    const char* command,
    const char* arg1,
    const char* arg2,
    FSNode* current
    )
{
    //check for argument errors
    if (validate_args(command, "arg1", arg2, SINGLE_ARG) == Error) {
        return;
    }
}


void process_cp(
    const FileSystem* system,
    const char* command,
    const char* arg1,
    const char* arg2,
    FSNode* current
    )
{
    //check for argument errors
    if (validate_args(command, "arg1", arg2, SINGLE_ARG) == Error) {
        return;
    }
}







