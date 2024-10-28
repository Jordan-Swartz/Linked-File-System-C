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

FSNode* root = NULL;
FSNode* current = NULL;
FSNode* previous = NULL;

/**
 *
 */
void system_setup() {
    //create root
    root = (FSNode*)malloc(sizeof(FSNode));

    //set up root
    printf("Enter root name: ");
    scanf("%s", root->name);
    root->type = Directory;
    root->permissions = r_w_e;
    root->size = 0;
    root->child_head = NULL;
    root->next = NULL;
    root->parent = NULL;

    current = root;
    previous = NULL;
    printf("Root directory %s initialized.\n", root->name);
}

/**
 *
 */
void create_node() {
    //check current directory type
    if (current->type == File) {
        printf("Error: Cannot create a new node inside a file.");
        return;
    }

    //create node
    FSNode* new_node = (FSNode*)malloc(sizeof(FSNode));

    printf("Enter new node name: ");
    scanf("%s", new_node->name);
    printf("What type of node would you like to create? (Directory=0, File=1) ");
    scanf("%d", new_node->type);

    new_node->permissions = r_w_e;
    new_node->child_head = NULL;
    new_node->next = NULL;
    new_node->parent = current;

    //add to front of current->child_head list if empty
    if (current->child_head == NULL) {
        new_node->next = current->child_head;
        current->child_head = new_node;
        printf("New node added.\n");
        return;
    }

    //FIX ME: add in condiiton to check next char if val[0] == val[0]

    //traverse head's children for alphabetical insert
    FSNode* iter = current->child_head;
    FSNode* iter_previous = NULL;

    while (iter != NULL) {
        if (strcmp(iter->name, new_node->name) > 0) {
                //insert in front
            if (iter_previous == NULL) {
                new_node->next = current->child_head;
                current->child_head = new_node;
            } else {
                //insert between previous and current
                new_node->next = iter;
                iter_previous->next = new_node;
            }
            printf("New node added.\n");
            return;

        } else if (strcmp(iter->name, new_node->name) == 0) {
            printf("Error: A node with this name already exists.\n");
            free(new_node);
            return;
        }

        //traverse to next node
        iter_previous = iter;
        iter = iter->next;
    }

    //insert at end
    iter_previous->next = new_node;
    printf("New node added at the end.\n");
}

/**
 *
 */
void delete_node() {

}

//TEST DELETE ME
void set_current() {

}









