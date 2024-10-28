//
// Created by jordan on 10/26/24.
//

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

/*Data Structures*/
//node type
typedef enum {
    Directory,
    File
} NodeType;

//permissions
typedef enum {
    r___,
    r_w_,
    r_w_e
} Permissions;

//file node
typedef struct {
    char name[100];
    NodeType type;
    Permissions permissions;
    int size;
    struct FSNode* child_head;
    struct FSNode* next;
    struct FSNode* parent;
} FSNode;

/*Global Variables*/
//root and current
extern FSNode* root;
extern FSNode* current;

//functions
void system_setup();
void create_node();
void delete_node();
void set_current();

#endif //FILE_SYSTEM_H