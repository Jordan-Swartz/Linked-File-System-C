/**
 * File manipulation and interaction:
 *
 * open/close file
 * read/write file
 * log results
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "file_system.h"
#include "file_operation.h"

/*Global Variables*/
char* menu_content = NULL;

/**
 *
 */
void system_load(FileSystem* system) {
    //open system file to read
    FILE* file = fopen("", "r");

    if (file == NULL) {
        system_setup(system);
        return;
    }

    fclose(file);
}

/**
 *
 */
void system_save(FileSystem* system) {
    //open system file to write
    FILE* file = fopen("", "w");
    fclose(file);
}

/**
 *
 */
void load_menu() {
    FILE* file = fopen("../data/menu.txt", "r");

    if (file == NULL) {
        printf("Error: file not found\n");
        return;
    }

    //get size of file
    fseek(file, 0, SEEK_END);             //move pointer to the end
    long file_size = ftell(file);                   //collect pointer bytes (size)
    rewind(file);                                   //move back to beginning

    //read from file
    menu_content = malloc(file_size + 1);           //allocate memory for file + '\0'

    if (menu_content == NULL) {
        printf("Error: could not allocate memory for menu\n");
        fclose(file);
        return;
    }

    fread(menu_content, 1, file_size, file);  //read file into menu buffer
    menu_content[file_size] = '\0';                 //fill last index with '\0'

    fclose(file);                                   //close file
}

/**
 *
 */
void free_menu() {
    free(menu_content);
    menu_content = NULL;
}