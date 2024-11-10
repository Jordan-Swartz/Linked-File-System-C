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
void root_setup(FileSystem* system, FSNode* root) {
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
void create_node(FileSystem* system, FSNode* current) {
    //check current directory type
    if (current->type == File) {
        printf("Error: Cannot create a new node inside a file.");
        return;
    }

    //create node
    FSNode* new_node = (FSNode*)malloc(sizeof(FSNode));

    //populate attributes
    printf("Enter new node name: ");
    scanf("%s", new_node->name);
    printf("What type of node would you like to create? (Directory=0, File=1) ");
    scanf("%u", &new_node->type);

    strcpy(new_node->owner, system->hostname);
    new_node->permissions = Read_Write;
    new_node->child_head = NULL;
    new_node->next = NULL;
    new_node->previous = NULL;
    new_node->parent = current;

    //add to front of current->child_head list if empty
    if (current->child_head == NULL) {
        current->child_head = new_node;
        printf("New node added.\n");
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
            printf("New node added.\n");
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
    printf("New node added at the end.\n");

}

/**
 *
 */
void delete_node() {

}

//TEST DELETE ME
void set_current() {
    //test
}









