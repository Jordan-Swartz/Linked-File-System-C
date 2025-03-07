/**
 * @file ui.h
 * @brief Defines the structs and methods for the processing of user commands.
 *
 * This header file declares functions for handling user input, executing file system operations,
 * and managing navigation within the system. It includes validation mechanisms and parsing utilities
 * for command execution.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025-02-20
 */

#ifndef UI_H
#define UI_H

#include "file_system.h"
#include "memory_management.h"

//traversal options
#define STOP_AT_LAST 1
#define FULL_TRAVERSAL 0

//node creation options
#define ENABLE_CREATE 1
#define DISABLE_CREATE 0

//node naming options
#define ENABLE_NAME 1
#define DISABLE_NAME 0

//arg condition options
#define SINGLE_ARG 1
#define DOUBLE_ARG 0

//node return option
#define IGNORE_RESULT NULL

//return status
typedef enum { Success, Error, Error_File, Exit } InputReturn;

/*Function Declarations*/
/**
 * Displays the file system menu.
 * Prints an error message if content is not loaded
 *
 */
void display_menu();

/**
 * Processes user input commands and executes corresponding file system operations.
 *
 * @param system Pointer to the file system.
 * @param current Pointer to the current directory.
 * @param history_stack Stack that holds the session commands.
 * @return Status indicating success, error, or exit.
 */
int process_input_command(const FileSystem* system, FSNode** current, Stack* history_stack);

/**
 * Validates the number of arguments provided for a command.
 *
 * @param command Command being executed.
 * @param arg1 First argument.
 * @param arg2 Second argument.
 * @param arg_condition required number of arguments.
 * @return Success if valid, otherwise an error.
 */
int validate_args(const char* command,
                  const char* arg1,
                  const char* arg2,
                  const int arg_condition
);

/**
 * Parses a given file path and traverses the file system accordingly.
 * Handles relative and absolute paths, and optionally creates missing directories.
 *
 * @param system Pointer to the file system.
 * @param path File path to process.
 * @param start_node Starting node for traversal.
 * @param return_node Output parameter for the node reached after traversal.
 * @param return_name Output parameter for the last segment of the parsed path.
 * @param stop_at_second_last Flag indicating whether to stop at the second-to-last node in the path.
 * @param enable_create_node Flag to enable or disable node creation during traversal.
 * @param enable_name_node Flag to validate node naming conventions.
 * @return Success if traversal succeeds, otherwise an error.
 */
int process_parsed_path(const FileSystem* system,
                        const char* path,
                        FSNode* start_node,
                        FSNode** return_node,
                        char** return_name,
                        const int stop_at_second_last,
                        const int enable_create_node,
                        const int enable_name_node
);

/**
 * Creates a new directory within the file system.
 *
 * @param system Pointer to the file system.
 * @param command The mkdir command.
 * @param arg1 The new directory path.
 * @param arg2 Unused parameter.
 * @param current Pointer to the current directory.
 */
void process_mkdir(const FileSystem* system,
                   const char* command,
                   const char* arg1,
                   const char* arg2,
                   FSNode* current
);

/**
 * Creates a new file in the specified destination directory.
 *
 * @param system Pointer to the file system.
 * @param command The touch command.
 * @param arg1 The path-to-destination (file parent).
 * @param arg2 The file name.
 * @param current Pointer to the current directory.
 */
void process_touch(const FileSystem* system,
                   const char* command,
                   const char* arg1,
                   const char* arg2,
                   FSNode* current
);

/**
 * Deletes a file or directory from the file system.
 *
 * @param system Pointer to the file system.
 * @param command The rm command.
 * @param arg1 The path-to-destination (deletion node).
 * @param arg2 Unused parameter.
 * @param current Pointer to the current directory.
 */
void process_rm(const FileSystem* system,
                const char* command,
                const char* arg1,
                const char* arg2,
                FSNode** current
);

/**
 * Prints the current directory path.
 *
 * @param current Pointer to the current directory.
 * @param root Pointer to the root directory.
 */
void process_pwd(FSNode* current, FSNode* root);

/**
 * Changes the current directory to the specified path.
 *
 * @param system Pointer to the file system.
 * @param command The cd command.
 * @param arg1 The path-to-destination (new current).
 * @param arg2 Unused parameter.
 * @param current Pointer to the current directory.
 */
void process_cd(const FileSystem* system,
                const char* command,
                const char* arg1,
                const char* arg2,
                FSNode** current
);

/**
 * Lists the contents of the specified directory.
 *
 * @param system Pointer to the file system.
 * @param command The ls command.
 * @param arg1 The path-to-destination (contents node) or NULL for current directory.
 * @param arg2 Unused parameter.
 * @param current Pointer to the current directory.
 */
void process_ls(const FileSystem* system,
                const char* command,
                const char* arg1,
                const char* arg2,
                FSNode* current
);

/**
 * Moves a file or directory to a new location.
 *
 * @param system Pointer to the file system.
 * @param command The mv command.
 * @param arg1 The path-to-source (relocate node).
 * @param arg2 The path-to-destination (new parent)
 * @param current Pointer to the current directory.
 */
void process_mv(const FileSystem* system,
                const char* command,
                const char* arg1,
                const char* arg2,
                FSNode* current
);

/**
 * Renames a file or directory.
 *
 * @param system Pointer to the file system.
 * @param command The rn command.
 * @param arg1 The path-to-source (rename node).
 * @param arg2 The new name.
 * @param current Pointer to the current directory.
 */
void process_rn(const FileSystem* system,
                const char* command,
                const char* arg1,
                const char* arg2,
                FSNode* current
);

/**
 * Changes the permissions of a file.
 *
 * @param system Pointer to the file system.
 * @param command The chmod command.
 * @param arg1 The permissions string.
 * @param arg2 The path-to-source (node to change).
 * @param current Pointer to the current directory.
 */
void process_chmod(const FileSystem* system,
                   const char* command,
                   const char* arg1,
                   const char* arg2,
                   FSNode* current
);

/**
 * Copies a file or directory to a new location.
 *
 * @param system Pointer to the file system.
 * @param command The cp command.
 * @param arg1 The path-to-source (copy node).
 * @param arg2 The path-to-destination (new parent).
 * @param current Pointer to the current directory.
 */
void process_cp(const FileSystem* system,
                const char* command,
                const char* arg1,
                const char* arg2,
                FSNode* current);


/**
 * Searches for a file or directory within the file system.
 * Prints out all possible paths to that node if found (from start node)
 *
 * @param system Pointer to the file system.
 * @param command The find command.
 * @param arg1 The path-to-source (start node).
 * @param arg2 The target name.
 * @param current Pointer to the current directory.
 */
void process_find(const FileSystem* system,
                  const char* command,
                  const char* arg1,
                  const char* arg2,
                  FSNode* current
);

/**
 * Displays the partial (last 5) or full command history of the session.
 *
 * @param history_stack
 * @param command The history command.
 * @param arg1 The flag for partial (-p) or full history (-f).
 * @param arg2 Unused parameter.
 */
void process_history(Stack* history_stack,
                    const char* command,
                    const char* arg1,
                    const char* arg2
);


#endif //UI_H
