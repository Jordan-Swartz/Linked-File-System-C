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
typedef enum { Directory, File } NodeType;

//permissions
typedef enum { Read, Read_Write, Read_Write_Execute } Permissions;

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
void root_setup(const FileSystem* system, FSNode* root);
void create_node(const FileSystem* system, FSNode* current, const char* name, const int type);
void delete_node();
void display_current_path(const FileSystem* system, const FSNode* current);
void display_directory_nodes(const FileSystem* system, const FSNode* current);
int change_directory();
void set_current();

#endif //FILE_SYSTEM_H