/**
* @file file_system_tests.h
 * @brief Defines the test methods for the file system core logic.
 *
 * This header file declares unit test functions for validating the core logic of the file system.
 * The tests ensure that the core logic works as intended.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025
 */

#ifndef FILE_SYSTEM_TESTS_H
#define FILE_SYSTEM_TESTS_H

#include "unity.h"
#include "file_system.h"
#include <stdlib.h>
#include <string.h>
#include <ui.h>

/**
 * Tests system_setup function by initializing system and verifying attributes.
 */
void test_system_setup(void);

/**
 * Tests root_setup function setting and verifying the root node attributes.
 */
void test_root_setup(void);

/**
 * Tests create_node function by creating a FSNode.
 */
void test_create_node(void);

/**
 * Tests create_copy_node function by duplicating FSNode attributes.
 */
void test_create_copy_node(void);

/**
 * Tests insert_node function to ensure nodes are inserted alphabetically.
 * Verifies that duplicates are not added.
 */
void test_insert_node(void);

/**
 * Tests recursive_copy function to validate deep copying of directories.
 */
void test_recursive_copy(void);

/**
 * Tests recursive_delete function to verify directory contents are freed without crashing.
 */
void test_recursive_delete(void);

/**
 * Tests change_directory_forward function for navigating directories.
 */
void test_change_directory_forward(void);

/**
 * Tests is_subdirectory function to ensure correct subdirectory detection.
 */
void test_is_subdirectory(void);

/**
 * Tests find_node function to ensure nodes are correctly found.
 */
void test_find_node(void);

#endif //FILE_SYSTEM_TESTS_H
