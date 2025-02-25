#include "main_program.h"
#include <stdio.h>
#include "file_system.h"
#include "file_operation.h"
#include "ui.h"

#include <ncurses.h>
#include <panel.h>
#include <form.h>
#include <stdlib.h>
#include <string.h>

// int main(int argc, char** argv) {
//     //load menu
//     load_menu();
//
//     //create and initialize system
//     FileSystem system;
//
//     // if (argc == 2) {
//     //     char* existing_system = (char*)argv[1];
//     //     system_load(&system, existing_system);
//     // } else {
//     //     printf("Error: missing file for system\n");
//     //     return Error;
//     // }
//
//     //TEST
//     FILE *file = fopen("/home/jordan/CLionProjects/Linked-File-System-C/data/test.txt", "r");
//     if (file == NULL) {
//         printf("Error: file not found\n");
//         return 1;
//     }
//     // The file exists, so close it
//     printf("File opened\n");
//     fclose(file);
//
//     // Now pass the file path (string) to system_load
//     char* const existing_system = "/home/jordan/CLionProjects/Linked-File-System-C/data/test.txt";
//     system_load_from_json(&system, existing_system);
//
//     //set current to system root
//     FSNode* current = system.root;
//
//     //clear buffer before processing input
//     int ch;
//     while ((ch = getchar()) != '\n' && ch != Error)
//         display_menu();
//
//     //process input
//     do {
//         //display_menu();
//     } while (process_input_command(&system, &current) != Exit);
//
//     //save system state, free memory and end program
//     system_save_to_json(&system, existing_system);
//     free_menu();
//     return 0;
// }


void input_popup(const char *title) {
    int height = 10, width = 40;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    // Create the popup window
    WINDOW *popup = newwin(height, width, starty, startx);
    box(popup, 0, 0);
    mvwprintw(popup, 1, (width - strlen(title)) / 2, "%s", title);
    refresh();
    wrefresh(popup);

    // Create form elements
    FIELD *fields[2];
    fields[0] = new_field(1, 30, 3, 5, 0, 0);
    fields[1] = NULL; // End of form fields

    set_field_back(fields[0], A_UNDERLINE);
    field_opts_off(fields[0], O_AUTOSKIP);

    // Create form and associate with window
    FORM *form = new_form(fields);
    set_form_win(form, popup);
    set_form_sub(form, derwin(popup, 1, 30, 3, 5));
    post_form(form);
    wrefresh(popup);

    int ch;
    while ((ch = getch()) != '\n') {  // Wait for Enter key
        form_driver(form, ch);
        wrefresh(popup);
    }

    // Get user input
    form_driver(form, REQ_NEXT_FIELD);
    form_driver(form, REQ_PREV_FIELD);
    char input[100];
    strncpy(input, field_buffer(fields[0], 0), sizeof(input) - 1);
    input[sizeof(input) - 1] = '\0';

    // Display user input
    mvwprintw(popup, 6, 5, "You entered: %s", input);
    wrefresh(popup);

    getch();  // Wait before closing

    // Cleanup
    unpost_form(form);
    free_form(form);
    free_field(fields[0]);
    delwin(popup);
}

int main(int argc, char** argv) {
    // ✅ Initialize ncurses properly
    initscr();      // Start ncurses mode
    raw();          // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys
    noecho();       // Don't echo user input
    curs_set(1);    // Show cursor

    // ✅ Fix issue by forcing proper drawing
    clear();
    refresh();

    // Display the input popup
    input_popup("Enter a command:");

    // Exit ncurses mode
    endwin();
    return 0;
}