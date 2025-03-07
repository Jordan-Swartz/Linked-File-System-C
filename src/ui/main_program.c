#include "main_program.h"
#include <stdio.h>
#include "file_system.h"
#include "file_operation.h"
#include "ui.h"

int main(int argc, char** argv) {
    //load menu
    load_menu();

    //create and initialize system
    FileSystem system;

    //create current session's history stack
    Stack history_stack;
    init_stack(&history_stack);

    // if (argc == 2) {
    //     char* existing_system = (char*)argv[1];
    //     system_load(&system, existing_system);
    // } else {
    //     printf("Error: missing file for system\n");
    //     return Error;
    // }

    //TEST
    FILE *file = fopen("/home/jordan/CLionProjects/Linked-File-System-C/data/test.txt", "r");
    if (file == NULL) {
        printf("Error: file not found\n");
        return 1;
    }
    // The file exists, so close it
    printf("File opened\n");
    fclose(file);

    // Now pass the file path (string) to system_load
    char* const existing_system = "/home/jordan/CLionProjects/Linked-File-System-C/data/test.txt";
    system_load_from_json(&system, existing_system);

    //set current to system root
    FSNode* current = system.root;

    //clear buffer before processing input
    int ch;
    while ((ch = getchar()) != '\n' && ch != Error)
        display_menu();

    //process input
    do {
        //display_menu();
    } while (process_input_command(&system, &current, &history_stack) != Exit);

    //save system state, free memory and end program
    system_save_to_json(&system, existing_system);
    free_menu();
    free_stack(&history_stack);
    return 0;
}
