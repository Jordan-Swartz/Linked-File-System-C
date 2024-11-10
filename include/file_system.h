//
// Created by jordan on 10/26/24.
//

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

/*Data Structures*/

/*Type Definitions*/
typedef struct FSNode FSNode;
typedef struct FileSystem FileSystem;

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
struct FSNode {
    char name[100];
    NodeType type;
    Permissions permissions;
    int size;
    FSNode* child_head;
    FSNode* next;
    FSNode* previous;
    FSNode* parent;
};

struct FileSystem {
    char hostname[100];
    FSNode* root;
};

/*Global Variables*/
//root and current
// extern FSNode* root;
// extern FSNode* current;

//functions
void system_setup(FileSystem* system);
void root_setup(FSNode* root);
void create_node();
void delete_node();
void set_current();

#endif //FILE_SYSTEM_H