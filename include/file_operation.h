/**
 * @file file_operation.h
 * @brief Defines the methods for the file system serialization and deserialization.
 *
 * This header file provides function declarations for converting file system nodes (FSNode)
 * into JSON format for saving and loading from a persistent storage file.
 * It also includes functions for handling menu content loading.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025-02-20
 */

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include <cJSON/cJSON.h>

/*Global Variables*/
extern char* menu_content;

/*Function Declarations*/
/**
 * Recursively converts a cJSON object into an FSNode.
 *
 * @param json_node Pointer to the cJSON object to parse.
 * @param parent Pointer to the parent FSNode (NULL if root).
 * @return Pointer to the created FSNode.
 */
FSNode* json_to_fsnode(const cJSON* json_node, FSNode* parent);

/**
 * Loads a file system from a JSON file and constructs the FSNode hierarchy.
 *
 * @param system Pointer to the FileSystem structure to populate.
 * @param existing_system Path to the JSON file containing the file system data.
 */
void system_load_from_json(FileSystem* system, const char* existing_system);

/**
 * Recursively converts an FSNode structure into a cJSON object.
 *
 * @param node Pointer to the FSNode structure to convert.
 * @return Pointer to a cJSON object representing the file system node.
 */
cJSON* fsnode_to_json(const FSNode* node);

/**
 * Saves the entire file system to a JSON file.
 *
 * @param system Pointer to the FileSystem structure to save.
 * @param existing_system Path to the JSON file where the data should be written.
 */
void system_save_to_json(FileSystem* system, const char* existing_system);

/**
 * Loads the menu from a text file into memory.
 */
void load_menu();

/**
 * Frees the allocated memory for menu content.
 */
void free_menu();

#endif //FILE_OPERATION_H


