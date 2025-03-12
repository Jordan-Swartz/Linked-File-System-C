/**
 * @file file_system.h
 * @brief Defines the structs and methods for the core file system operations.
 *
 * This header file provides the necessary declarations for creating, manipulating,
 * and navigating a hierarchical file system structure. It supports traditional
 * file operations such as creation, deletion, copying, searching, and directory traversal.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025-02-20
 */

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "stack.h"

/*Data Structures*/
/*Type Definitions*/
typedef struct FSNode FSNode;
typedef struct FileSystem FileSystem;
typedef enum { Directory, File } NodeType;                          //node type
typedef enum { Read, Read_Write, Read_Write_Execute } Permissions;  //permissions

/**
 * @struct FSNode
 * @brief Represents a file or directory within the file system.
 */
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

/**
 * @struct FileSystem
 * @brief Represents the entire file system.
 */
struct FileSystem {
    char username[100];
    char host_signature[100];
    FSNode* root;
};

/*Global Variables*/
extern const char* NodeTypeNames[];
extern const char* PermissionsNames[];

/*Function Declarations*/
/**
 * Initializes the file system.
 *
 * @param system Pointer to the file system.
 */
void system_setup(FileSystem* system);

/**
 * Sets up the root directory of the file system.
 *
 * @param system Pointer to the file system.
 * @param root Pointer to the root node.
 */
void root_setup(const FileSystem* system, FSNode* root);

/**
 * Creates a new file or directory node.
 *
 * @param system_username Username of the system owner.
 * @param current Pointer to the current directory.
 * @param name Name of the new node.
 * @param type Type of node (Directory or File).
 * @param permissions Permissions of the node.
 * @return Pointer to the newly created node.
 */
FSNode* create_node(const char* system_username,
                    FSNode* current,
                    const char* name,
                    NodeType type,
                    Permissions permissions
);

/**
 * Creates a copy of an existing node.
 *
 * @param current Pointer to the current directory.
 * @param destination Pointer to the target directory where the copy will be placed.
 * @return Pointer to the copied node.
 */
FSNode* create_copy_node(FSNode* current, FSNode* destination);

/**
 * Recursively deletes a node and all its children.
 *
 * @param current Pointer to the current directory.
 */
void recursive_delete(FSNode* current);

/**
 * Recursively copies a node and all its children.
 *
 * @param current Pointer to the current directory.
 * @param destination Pointer to the target directory where the copy will be placed.
 */
void recursive_copy(FSNode* current, FSNode* destination);

/**
 * Performs a depth-first search (DFS) to find a node by name.
 *
 * @param start Pointer to the starting directory for the search.
 * @param name Name of the target node.
 * @return Array of strings containing paths to all matching nodes.
 */
char** dfs_search(FSNode* start, char* name);

/**
 * Displays the full path of the current directory.
 *
 * @param system Pointer to the file system.
 * @param current Pointer to the current directory.
 */
void display_current_path(const FileSystem* system, FSNode* current);

/**
 * Displays the contents of a directory.
 *
* @param system Pointer to the file system.
 * @param current Pointer to the current directory.
 */
void display_directory_nodes(const FileSystem* system, const FSNode* current);

/**
 * Moves the current directory forward into a subdirectory.
 *
 * @param current Pointer to the current directory.
 * @param change_to_name Name of the subdirectory to move into.
 * @return
 */
int change_directory_forward(FSNode** current, char* change_to_name);

/**
 * Moves the current directory backward to the parent directory.
 *
 * @param current Pointer to the current directory.
 */
void change_directory_backward(FSNode** current);

/**
 * Sets the current directory to a specified node.
 *
 * @param current Pointer to the current directory.
 * @param change_to_node Pointer to the new current directory.
 */
void set_current(FSNode** current, FSNode* change_to_node);

/**
 * Changes the permissions of a specified node.
 *
 * @param node The node that is being modified.
 * @param permissions The permissions to modify with.
 */
void change_node_permissions(FSNode* node, Permissions permissions);

/**
 * Parses a file path into individual directory names.
 *
 * @param argument Path to be parsed.
 * @return Array of parsed strings representing each directory in the path.
 */
char** parse_path(const char* argument);

/**
 * Builds a full path string from a starting node to an endpoint.
 *
 * @param start Pointer to the starting directory.
 * @param end Pointer to the target directory.
 * @return Full path string.
 */
char* build_path(FSNode* start, FSNode* end);

/**
 * Inserts a node into a directory in alphabetical order.
 *
 * @param current Pointer to the current directory.
 * @param insert_node Pointer to the node to insert.
 * @return Success if inserted successfully, otherwise Error.
 */
int insert_node(FSNode* current, FSNode* insert_node);

/**
 * Checks if destination node is a subdirectory of the source node.
 *
 * @param source Pointer to the potential parent directory.
 * @param destination Pointer to the directory to check.
 * @return Error if it is a subdirectory, otherwise Success.
 */
int is_subdirectory(FSNode* source, FSNode* destination);

/**
 * Finds a node by name within a directory.
 *
 * @param current Pointer to the current directory.
 * @param name Name of the target node.
 * @return Pointer to the found node, or NULL if not found.
 */
FSNode* find_node(FSNode* current, char* name);

#endif //FILE_SYSTEM_H