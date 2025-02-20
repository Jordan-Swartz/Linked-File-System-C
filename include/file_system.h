#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "stack.h"

/*Data Structures*/
/*Type Definitions*/
typedef struct FSNode FSNode;
typedef struct FileSystem FileSystem;
typedef enum { Directory, File } NodeType;                          //node type
typedef enum { Read, Read_Write, Read_Write_Execute } Permissions;  //permissions

/*File Node*/
struct FSNode {
    char name[150];
    char owner[100];
    NodeType type;
    Permissions permissions;
    int size;
    FSNode* child_head;
    FSNode* next;
    FSNode* previous;
    FSNode* parent;
};

/*File System*/
struct FileSystem {
    char username[100];
    char host_signature[100];
    FSNode* root;
};

/*Global Variables*/
extern const char* NodeTypeNames[];
extern const char* PermissionsNames[];

/*Function Declarations*/
void system_setup(FileSystem* system);
void root_setup();
FSNode* create_node(const char* system_username, FSNode* current, const char* name, NodeType type);
FSNode* create_copy_node();
void recursive_delete();
void recursive_copy();
char** dfs_search();
void display_current_path();
void display_directory_nodes();
int change_directory_forward();
void change_directory_backward();
void set_current();
char** parse_path();
char* build_path();
int insert_node();
int is_subdirectory();
FSNode* find_node();

#endif //FILE_SYSTEM_H