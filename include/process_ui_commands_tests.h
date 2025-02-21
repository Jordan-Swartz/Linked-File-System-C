/**
 * @file process_ui_commands_tests.h
 * @brief Defines the test methods for processing ui commands.
 *
 * This header file declares unit test functions for validating argument parsing
 * and command processing in the user interface of the file system.
 * The tests ensure that command validation behaves correctly under different conditions.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025-02-20
 */

#ifndef PROCESS_UI_COMMANDS_TESTS_C_H
#define PROCESS_UI_COMMANDS_TESTS_C_H

#include "unity.h"
#include "ui.h"

/**
 * Tests if a command with a single valid argument passes validation.
 */
void test_validate_args_single_arg_success(void);

/**
 * Tests if a command with a single valid argument passes validation.
 */
void test_validate_args_single_arg_fail(void);

#endif //PROCESS_UI_COMMANDS_TESTS_C_H
