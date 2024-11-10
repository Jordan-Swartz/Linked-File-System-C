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
    Read,
    Read_Write,
    Read_Write_Execute
} Permissions;

//file node
struct FSNode {
    char name[100];
    char owner[100];
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
    char host_signature[100];
    FSNode* root;
};

/*Global Variables*/
extern const char* NodeTypeNames[];
extern const char* PermissionsNames[];

//functions
void system_setup(FileSystem* system);
void root_setup(FileSystem* system, FSNode* root);
void create_node(FileSystem* system, FSNode* current);
void delete_node();
void set_current();

#endif //FILE_SYSTEM_H