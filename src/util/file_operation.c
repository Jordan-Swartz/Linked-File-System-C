#include <stdio.h>
#include <stdlib.h>
#include "file_system.h"
#include "file_operation.h"

/*Global Variables*/
char* menu_content = NULL;

int file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0); // Returns 1 if exists, 0 otherwise
}

FSNode* json_to_fsnode(const cJSON* json_node, FSNode* parent) {
    //TODO
    return NULL;
}

void system_load_from_json(FileSystem* system, const char* existing_system) {
    //open system file to read
    FILE* file = fopen(existing_system, "r");

    //get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    //if file is empty create new system
    if (file_size == 0) {
        system_setup(system);
        fclose(file);
        return;
    }

    //load existing system

    fclose(file);
}

cJSON* fsnode_to_json(const FSNode* node) {
    if (node)
    return NULL;
}

void system_save_to_json(FileSystem* system, const char* existing_system) {
    //open system file to write
    FILE* file = fopen("", "w");
    if (!file) {
        printf("Error: Could not open file for saving/.\n");
        return;
    }

    //convert system into json object
    cJSON* json_system = fsnode_to_json((system->root));

    //convert json object to string and write to file
    char* json_string = cJSON_Print(json_system);
    fprintf(file, "%s", json_string);
    fclose(file);

    //clean memory
    cJSON_Delete(json_system);
    free(json_string);
}

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

void free_menu() {
    free(menu_content);
    menu_content = NULL;
}