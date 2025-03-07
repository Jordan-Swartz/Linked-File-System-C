#include <stdio.h>		//I/O features
#include <string.h>		//String lib
#include "ui.h"
#include <stdlib.h>
#include "file_system.h"
#include "file_operation.h"

void display_menu() {
    if (menu_content != NULL) {
        printf("\n%s\n", menu_content);
    } else {
        printf("Error: menu content not loaded\n");
    }
}

int process_input_command(const FileSystem* system, FSNode** current, Stack* history_stack) {
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

    //add command to history stack
    push(history_stack, (void*)command, STACK_STRING);

    //default return status
    int status = Success;

    if (command == NULL) {
        printf("Error: No command entered\n");
        return Error;
    }

    //process make directory
    if (strcmp(command, "mkdir") == 0) {
        process_mkdir(system, command, argument, argument2, (*current));
    }

    //process make file
    else if (strcmp(command, "touch") == 0) {
        process_touch(system, command, argument, argument2, (*current));
    }

    //process delete file or directory
    else if (strcmp(command, "rm") == 0) {
         process_rm(system, command, argument, argument2, current);
    }

    //process display current path
    else if (strcmp(command, "pwd") == 0) {
        process_pwd((*current), system->root);
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
        process_mv(system, command, argument, argument2, (*current));
    }

    //process rename file or directory
    else if (strcmp(command, "rn") == 0) {
        process_rn(system, command, argument, argument2, (*current));
    }

    //process changing permissions for file
    else if (strcmp(command, "chmod") == 0) {

    }

    //process copying file or directory into a new destination
    else if (strcmp(command, "cp") == 0) {
        process_cp(system, command, argument, argument2, (*current));
    }

    //process finding file or directory in current directory
    else if (strcmp(command, "find") == 0) {
        process_find(system, command, argument, argument2, (*current));
    }

    //process displaying command history for the current session
    else if (strcmp(command, "history") == 0) {
        process_history(history_stack, command, argument, argument2);
    }

    //process cat
    else if (strcmp(command, "cat") == 0) {

    }

    //process displaying menu
    else if (strcmp(command, "menu") == 0) {
        display_menu();
    }

    //process exit system
    else if (strcmp(command, "exit") == 0)  {
        printf("Exiting System...\n");
        status = Exit;
    }

    //process unknown command
    else {
        printf("Error: '%s' command not found\n", command);
        status = Error;
    }

    return status;
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
                FSNode* new_node = create_node(system->username, current, parsed_path[i], Directory);
                //insert node alphabetically
                if (insert_node(current, new_node) != Success) {
                    printf("Error: '%s' A node with this name already exists.\n", parsed_path[i]);
                    free(new_node);
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
    FSNode* new_node = create_node(system->username, destination, arg2, File);
    //insert node alphabetically
    if (insert_node(destination, new_node) != Success) {
        printf("Error: '%s' A node with this name already exists.\n", arg2);
        free(new_node);
    }
}

void process_rm(
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

    //traverse to find deletion directory
    FSNode* deletion_directory = (*current);
    if (process_parsed_path(system, arg1, (*current), &deletion_directory, NULL,
        FULL_TRAVERSAL, DISABLE_CREATE, DISABLE_NAME) == Error)
    {
        return;
    }

    //ensure deletion is not root
    if (deletion_directory == system->root) {
        printf("Error: Cannot delete the root directory.\n");
        return;
    }

    //continuation warning
    printf("Are you sure you would like to continue with deleting directory '%s'? "
           "Doing so will delete all of its contents. Insert [y/n] to continue.\n", deletion_directory->name);

    while (1) {
        char input = getchar();
        while (getchar() != '\n');

        if (input == 'y') {
            break;
        } else if (input == 'n') {
            printf("Cancelling deletion process.\n");
            return;
        } else {
            printf("Invalid input. Insert [y/n] to continue.\n");
        }
    }

    //update parent head if it is the removal directory
    if (deletion_directory->parent->child_head == deletion_directory) {
        deletion_directory->parent->child_head = deletion_directory->next;
    }
    //update removal directory's previous if not the head
    if (deletion_directory->previous != NULL) {
        deletion_directory->previous->next = deletion_directory->next;
    }
    //update removal directory's next if not the tail
    if (deletion_directory->next != NULL) {
        deletion_directory->next->previous = deletion_directory->previous;
    }

    //fall back to parent if current directory is the removal directory
    if (deletion_directory == (*current)) {
        printf("Warning: You are in the directory being deleted. Moving to the parent directory before processing deletion.\n");
        (*current) = deletion_directory->parent;
    }

    //decrement parent size and delete directory contents recursively
    deletion_directory->parent->size--;
    recursive_delete(deletion_directory);
}

void process_pwd(FSNode* current, FSNode* root) {
    char* path = build_path(current, root);
    printf("%s\n", path);
    free(path);
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
    //process ls by itself
    if (arg1 == NULL) {
        display_directory_nodes(system, current);
        return;
    }

    //check for argument errors
    if (validate_args(command, arg1, arg2, SINGLE_ARG) == Error) {
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
    if (validate_args(command, arg1, arg2, DOUBLE_ARG) == Error) {
        return;
    }

    //traverse to find copy node
    FSNode* copy_node = current;
    if (process_parsed_path(system, arg1, current, &copy_node, NULL,
        FULL_TRAVERSAL, DISABLE_CREATE, DISABLE_NAME) == Error)
    {
        return;
    }

    //ensure copy node is not root
    if (copy_node == system->root) {
        printf("Error: Cannot copy the root directory.\n");
        return;
    }

    //traverse to find destination node
    FSNode* destination_node = current;
    if (process_parsed_path(system, arg2, current, &destination_node, NULL,
        FULL_TRAVERSAL, DISABLE_CREATE, DISABLE_NAME) == Error)
    {
        return;
    }

    //ensure destination node is not the copy node
    if (copy_node == destination_node) {
        printf("Error: Cannot copy '%s' -> Destination is the same as the source.\n", copy_node->name);
        return;
    }

    //ensure destination node is not a child of the copy node
    if (is_subdirectory(copy_node, destination_node) != Success) {
        printf("Error: Cannot copy '%s' -> Destination is a subdirectory the source.\n", copy_node->name);
        return;
    }

    //ensure destination node is valid (no node with existing name)
    if (find_node(destination_node, copy_node->name) != NULL) {
        printf("Error: Cannot copy '%s' -> A node with this name already exists in the destination directory.\n", copy_node->name);
        return;
    }

    //continuation warning if directory
    if (copy_node->type == Directory) {
        printf("Are you sure you would like to continue with copying directory '%s'? "
               "Doing so will copy all of its contents. Insert [y/n] to continue.\n", copy_node->name);

        while (1) {
            char input = getchar();
            while (getchar() != '\n');

            if (input == 'y') {
                break;
            } else if (input == 'n') {
                printf("Cancelling copying process.\n");
                return;
            } else {
                printf("Invalid input. Insert [y/n] to continue.\n");
            }
        }
    }
    //copy node contents recursively
    recursive_copy(copy_node, destination_node);
}

void process_find(
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

    //traverse to find start node
    FSNode* start_node = current;
    if (process_parsed_path(system, arg1, current, &start_node, NULL,
        FULL_TRAVERSAL, DISABLE_CREATE, DISABLE_NAME) == Error)
    {
        return;
    }

    //search for find node type and return array of paths to find node
    char** paths_to_find_node = dfs_search(start_node, arg2);
    if (paths_to_find_node[0] == NULL) {
        printf("Error: Cannot find any paths to '%s'\n", arg2);
        free_path(paths_to_find_node);
        return;
    }

    //print list of paths to find node
    int i = 0;
    printf("Paths to '%s' starting from %s:\n", arg2, start_node->name);
    while (paths_to_find_node[i] != NULL) {
        printf("%s\n", paths_to_find_node[i]);
        i++;
    }
    free_path(paths_to_find_node);
}

void process_history(
    Stack* history_stack,
    const char* command,
    const char* arg1,
    const char* arg2
    )
{
    //check for argument errors
    if (validate_args(command, arg1, arg2, SINGLE_ARG) == Error) {
        return;
    }

    //validate arg flag
    if (strcmp(arg1, "-p") != 0 || strcmp(arg1, "-f") != 0) {
        return;
    }

    Stack temp, restore;
    init_stack(&temp);
    init_stack(&restore);
    int count = 0;

    //print session history
    printf("Session Commands:\n");
    while (is_empty(history_stack) != 1) {
        if (count == 5 && (strcmp(arg1, "-p") == 0)) {
            break;
        } else {
            char* item = (char*)pop(history_stack);
            push(&temp, item, STACK_STRING);
            printf("%s\n", item);
            count++;
        }
    }

    //move remaining elements to restore stack if (-p)
    while (!is_empty(history_stack)) {
        push(&restore, pop(history_stack), STACK_STRING);
    }
    //push back into history_stack in original order
    while (!is_empty(&restore)) {
        push(history_stack, pop(&restore), STACK_STRING);
    }
    while (!is_empty(&temp)) {
        push(history_stack, pop(&temp), STACK_STRING);
    }

    //FIXME

}







