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
        //check for argument errors
        if (argument2 != NULL) {
            printf("Error: '%s' too many arguments\n", command);
            return Error;
        } else if (argument == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }

        //ensure mkdir arg is not root directory
        if (strcmp(argument, "~") == 0 || strcmp(argument, "/") == 0) {
            printf("Error: Root directory already exists.\n");
            return Error;
        }

        //handle creation of nested directories
        FSNode* current_copy = NULL;
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

        while (parsed_path[i] != NULL) {
            //reject creating '.'
            if (strcmp(parsed_path[i], ".") == 0) {
                printf("Error: Cannot create directory named '%s'.\n", parsed_path[i]);
                free_path(parsed_path);
                return Error;
            }

            //process backwards change
            if (strcmp(parsed_path[i], "..") == 0) {
                if (strcmp(current_copy->name, system->root->name) == 0) {
                    printf("Error: Cannot move past the root directory.\n");
                    free_path(parsed_path);
                    return Error;
                }
                change_directory_backward(&current_copy);

            } else {
                //process forward change and attempt to create directory
                change_result = create_node(system, current_copy, parsed_path[i], Directory);

                if (change_result == Error) {
                    printf("Error: '%s' A node with this name already exists.\n", parsed_path[i]);
                    free_path(parsed_path);
                    return Error;
                } else if (parsed_path[i+1] != NULL) {
                    //change to new directory
                    change_directory_forward(&current_copy, parsed_path[i]);
                }
            }
            //move to next string
            i++;
        }

        //free parsed path
        free_path(parsed_path);
        return Success;
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
        FSNode* iter = (*current);
        char path[1024] = "";
        char temp[100];

        //create path
        while (iter != NULL) {
            sprintf(temp, "/%s%s", iter->name, path);
            strcpy(path, temp);
            iter = iter->parent;
        }

        printf("%s\n", path);
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

        //process change to root '~' or '/'
        if (strcmp(argument, "~") == 0 || strcmp(argument, "/") == 0) {
            (*current) = system->root;
            return Success;
        }

        //parse path
        FSNode* current_copy = NULL;
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

        while (parsed_path[i] != NULL) {
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

                if (change_result == Error_File) {
                    printf("Error: '%s' is not a directory.\n", parsed_path[i]);
                    free_path(parsed_path);
                    return Error;

                } else if (change_result == Error) {
                    printf("Error: '%s' -> No such file or directory\n", parsed_path[i]);
                    free_path(parsed_path);
                    return Error;
                }
            }
            //move to next string
            i++;
        }
        //free path array and update current
        (*current) = current_copy;
        free_path(parsed_path);
        return Success;
    }

    //process display directory contents
    else if (strcmp(command, "ls") == 0)  {
        //check for argument errors
        if (argument2 != NULL) {
            printf("Error: '%s' too many arguments\n", command);
            return Error;
        }

        //process ls by itself
        if (argument == NULL) {
            display_directory_nodes(system, (*current));
            return Success;
        }

        //process ls for root '~' or '/'
        if (strcmp(argument, "~") == 0 || strcmp(argument, "/") == 0) {
             display_directory_nodes(system, system->root);
            return Success;
        }

        //process ls after directory change
        FSNode* current_copy = NULL;
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

        while (parsed_path[i] != NULL) {
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

        //display directory contents and free path array
        display_directory_nodes(system, current_copy);
        free_path(parsed_path);
        return Success;
    }

    //process move file or directory
    else if (strcmp(command, "mv") == 0) {
        //check for argument errors
        if (argument == NULL || argument2 == NULL) {
            printf("Error: '%s' missing argument\n", command);
            return Error;
        }

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

        //reject renaming '..', '.'
        if (strcmp(argument2, "..") == 0 || strcmp(argument2, ".") == 0) {
            printf("Error: Cannot rename '%s'.\n", argument2);
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

        //rename node and free path array
        strcpy(rename_node->name, argument2);
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
        return Exit;
    }

    //process unknown command
    else {
        printf("Error: '%s' command not found\n", command);
        return Error;
    }
}











