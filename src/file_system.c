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
    printf("Enter system name: ");
    scanf("%s", system->hostname);
    strcpy(system->host_signature, system->hostname);
    strcat(system->host_signature, "@JDS:");

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
    strcpy(root->owner, system->hostname);
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
void create_node(const FileSystem* system, FSNode* current, const char* name,
                 const int type) {
    //create node
    FSNode* new_node = (FSNode*)malloc(sizeof(FSNode));

    //populate attributes
    strcpy(new_node->name, name);
    strcpy(new_node->owner, system->hostname);
    new_node->type = type;
    new_node->permissions = Read_Write;
    new_node->child_head = NULL;
    new_node->next = NULL;
    new_node->previous = NULL;
    new_node->parent = current;

    //add to front of current->child_head list if empty
    if (current->child_head == NULL) {
        current->child_head = new_node;
        printf("New node %s added.\n", name);
        current->size++;
        return;
    }

    //traverse head's children for alphabetical insert
    FSNode* iter = current->child_head;

    while (iter != NULL) {
        //if iter comes after new_node alphabetically
        if (strcmp(iter->name, new_node->name) > 0) {

            //link new node after previous and before iter
            new_node->next = iter;
            new_node->previous = iter->previous;;

            //insert in front of current head (iter)
            if (iter->previous == NULL) {
                current->child_head = new_node;
            } else {
                //insert between previous and current
                iter->previous->next = new_node;
            }

            //link iter previous to new node
            iter->previous = new_node;
            printf("New node %s added.\n", name);
            return;

        } else if (strcmp(iter->name, new_node->name) == 0) {
            printf("Error: A node with this name already exists.\n");
            free(new_node);
            return;
        }

        //traverse until last node
        if (iter->next == NULL) {
            break;
        } else {
            iter = iter->next;
        }
    }

    //insert at end
    iter->next = new_node;
    new_node->previous = iter;
    current->size++;
    printf("New node %s added.\n", name);

}

/**
 *
 */
void delete_node() {

}

/**
 * pwd move to file_system
 */
void display_current_path(const FileSystem* system, const FSNode* current) {
    char path[1024] = "$";
    char temp[100] = "";
    FSNode* iter = current;

    //create path starting from current and traversing up to the root
    while (iter != NULL) {
        sprintf(temp,"/%s%s", iter->name, path);
        strcpy(path, temp);
        iter = iter->parent;
    }

    //append system name to front of path
    sprintf(temp, "%s~%s", system->host_signature, path);
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
            iter->size, system->hostname, iter->name);
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











