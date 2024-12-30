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

struct FileSystem {
    char username[100];
    char host_signature[100];
    FSNode* root;
};

/*Global Variables*/
extern const char* NodeTypeNames[];
extern const char* PermissionsNames[];

//functions
void system_setup(FileSystem* system);
void root_setup();
int create_node();
void delete_node();
void display_current_path();
void display_directory_nodes();
int change_directory_forward();
void change_directory_backward();
void set_current();
char** parse_path();
int insert_node();
int is_subdirectory();
FSNode* find_node();

#endif //FILE_SYSTEM_H