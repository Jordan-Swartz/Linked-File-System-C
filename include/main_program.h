/**
 * @file main_program.h
 * @brief Defines the main method for program execution.
 *
 * This header file declares the entry point of the program and the function responsible
 * for running the main logic of the file system. It ensures proper initialization,
 * loading, and saving of the file system while handling user input commands.
 *
 * @author Jordan Swartz
 * @version 1.0
 * @date 2025-02-20
 */

#ifndef MAIN_PROGRAM_H
#define MAIN_PROGRAM_H

/*Function Declarations*/
/**
 * Entry point for the file system program.
 * Initializes the file system, loads data from a JSON file if available,
 * processes user commands, and saves the system state before exiting.
 *
 * @param argc Argument count.
 * @param argv Array of arguments.
 * @return Returns 0 on successful execution
 */
int main(int argc, char** argv);

#endif //MAIN_PROGRAM_H
