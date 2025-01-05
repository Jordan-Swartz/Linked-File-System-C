/**
 * core file system operations:
 *
 * create/delete file
 * create/delete directory
 * move files between directories
 * traverse directory (cd, cd ..)
 *
 * contain nodes/tree struct
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_system.h"

#include <ui.h>

/*Global Variables*/
const char* NodeTypeNames[] = {"D", "F"};
const char* PermissionsNames[] = {"R____", "R_W__", "R_W_E"};

/**
 *
 */
void system_setup(FileSystem* system) {
    //set up system
    printf("Enter system username: ");
    scanf("%s", system->username);
    strcpy(system->host_signature, system->username);
    strcat(system->host_signature, "@JDS");

    //allocate memory for root node and initialize
    system->root = (FSNode*)malloc(sizeof(FSNode));
    root_setup(system, system->root);
}

/**
 *
 */
void root_setup(const FileSystem* system, FSNode* root) {
    //set up root
    printf("Enter root name: ");
    scanf("%s", root->name);
    strcpy(root->owner, system->username);
    root->type = Directory;
    root->permissions = Read_Write;
    root->size = 0;
    root->child_head = NULL;
    root->next = NULL;
    root->previous = NULL;
    root->parent = NULL;
    printf("Root directory %s initialized.\n", root->name);
}

/**
 *
 */
int create_node(
    const FileSystem* system,
    FSNode* current,
    const char* name,
    NodeType type
    )
{
    //create node
    FSNode* new_node = (FSNode*)malloc(sizeof(FSNode));

    //populate attributes
    strcpy(new_node->name, name);
    strcpy(new_node->owner, system->username);
    new_node->type = type;
    new_node->permissions = Read_Write;
    new_node->child_head = NULL;
    new_node->next = NULL;
    new_node->previous = NULL;
    new_node->parent = current;

    //insert node alphabetically
    if (insert_node(current, new_node) == Error) {
        free(new_node);
        return Error;
    }
    return Success;
}

/**
 *
 */
void delete_node() {

}

/**
 * pwd move to file_system
 */
void display_current_path(const FileSystem* system, FSNode* current) {
    char path[1024] = "$";
    char temp[100] = "";

    //if in root directory (home)
    if (current == system->root) {
        printf("%s:~$ ", system->host_signature);
        return;
    }

    FSNode* iter = current;

    //create path starting from current and traversing up to the root
    while (iter != system->root) {
        sprintf(temp,"/%s%s", iter->name, path);
        strcpy(path, temp);
        iter = iter->parent;
    }

    //append system name to front of path
    sprintf(temp, "%s:~%s", system->host_signature, path);
    strcpy(path, temp);

    //display path
    printf("%s ", path);
    fflush(stdout);
}


/**
 * ls -la move to file_system
 */
void display_directory_nodes(const FileSystem* system, const FSNode* current) {
    FSNode* iter = current->child_head;

    //display size
    printf("size: %d\n", current->size);

    while (iter != NULL) {
        printf("%s_%s %d %s %s\n", NodeTypeNames[iter->type], PermissionsNames[iter->permissions],
            iter->size, system->username, iter->name);
        iter = iter->next;
    }
}

int change_directory_forward(FSNode** current, char* change_to_name) {
    FSNode* change_to_node = NULL;

    //traverse list for match
    FSNode* iter = (*current)->child_head;
    while (iter != NULL && iter->name[0] <= change_to_name[0]) {
        if (strcmp(iter->name, change_to_name) == 0) {
            change_to_node = iter;
            break;
        } else {
            iter = iter->next;
        }
    }

    //return if no node found or match is a file error
    if (change_to_node == NULL) {
        // printf("No match found\n");
        return Error;
    }
    else if (change_to_node->type == File) {
        return Error_File;
    }

    //change current
    // printf("Match found\n");
    *current = change_to_node;
    return Success;
}

void change_directory_backward(FSNode** current) {
    (*current) = (*current)->parent;
}


void set_current(FSNode** current, FSNode* change_to_node) {
    (*current) = change_to_node;
}

//return array of parsed strings
char** parse_path(const char* argument) {
    char temp[256] = {0};
    int i = 0, temp_index = 0, arr_index = 0;

    //array of strings
    char** parsed_path = (char**)malloc(sizeof(char*) * 256);

    //skip first char if abs path
    if (argument[0] == '/') {
        i++;
    }

    while (argument[i] != '\0') {
    //process if slash is reached and buffer isn't empty
        if (argument[i] == '/') {
            if (temp_index > 0) {
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
            }
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
        //printf("string %s\n", substring);
        arr_index++;
    }

    //null terminate array
    //printf("index %d\n", arr_index);
    parsed_path[arr_index] = NULL;
    return parsed_path;
}

//insert node alphabetically
int insert_node(FSNode* current, FSNode* node) {
    //increment size (rollback on failure)
    current->size++;

    //add to front of current->child_head list if empty
    if (current->child_head == NULL) {
        current->child_head = node;
        return Success;
    }

    //traverse head's children for alphabetical insert
    FSNode* iter = current->child_head;

    while (iter != NULL) {
        //if iter comes after new_node alphabetically
        if (strcmp(iter->name, node->name) > 0) {
            //link new node after previous and before iter
            node->next = iter;
            node->previous = iter->previous;

            //insert in front of current head (iter)
            if (iter->previous == NULL) {
                current->child_head = node;
            } else {
                //insert between previous and current
                iter->previous->next = node;
            }

            //link iter previous to new node
            iter->previous = node;
            return Success;

        } else if (strcmp(iter->name, node->name) == 0) {
            //node name already exists
            current->size--;
            return Error;
        }

        //traverse until last node
        if (iter->next == NULL) {
            break;
        } else {
            iter = iter->next;
        }
    }

    //insert at end
    iter->next = node;
    node->previous = iter;
    return Success;
}

//checks if destination node is a child of the source
int is_subdirectory(FSNode* source, FSNode* destination) {
    FSNode* current = destination;
    while (current != NULL) {
        if (current == source) {
            return Error;
        }
        current = current->parent;
    }
    return Success;
}

//returns pointer to desired node if found, NULL no node is found
FSNode* find_node(FSNode* current, char* change_to_name) {
    FSNode* iter = current->child_head;

    while (iter != NULL) {
        if (strcmp(iter->name, change_to_name) == 0) {
            return iter;
        } else {
            iter = iter->next;
        }
    }

    return NULL;
}









