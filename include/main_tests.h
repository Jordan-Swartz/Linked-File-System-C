/**
 * @file main_tests.h
 * @brief Defines the test methods for program validation.
 *
 * This header file declares the flow of test functions for validating the system's functionality,
 * including its ui and backend.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025
 */

#ifndef MAIN_TESTS_H
#define MAIN_TESTS_H

#include "unity.h"
#include "file_system.h"
#include "file_system_tests.h"
#include "ui_tests.h"

/**
 * Sets up the testing environment before each test.
 */
void setUp(void);

/**
 * Cleans up the testing environment after each test.
 */
void tearDown(void);

/**
 * Executes file system unit tests.
 * @return UNITY_END() - 0 if all passed, otherwise # of failed.
 */
int main(void);

#endif //MAIN_TESTS_H
