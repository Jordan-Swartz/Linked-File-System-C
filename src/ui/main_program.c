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

    char* existing_system;
    if (argc == 2) {
        //load existing file
        existing_system = (char*)argv[1];
        system_load_from_json(&system, existing_system);

    } else if (argc == 1) {
        //no file provided set to default
        printf("No File Provided.\n");
        existing_system = generate_unique_filename();

    }  else {
        //too many args
        printf("Error: too many input arguments.\n");
        return Error;
    }

    //if default file doesn't exist, create one
    if (!file_exists(existing_system)) {
        printf("System file not found. Creating a new one: %s\n", existing_system);
        //ensure data dir exists
        mkdir(DEFAULT_DIRECTORY, 0777);

        //create new file
        FILE* file = fopen(existing_system, "w");
        if (file) {
            fprintf(file, "{}"); //write empty obj
            fclose(file);
        } else {
            printf("Error: Could not create system file.\n");
            return Error;
        }

        //setup new system
        system_setup(&system);
    }

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
