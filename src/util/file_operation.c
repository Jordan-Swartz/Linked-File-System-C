#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file_system.h"
#include "file_operation.h"

#include <string.h>

/*Global Variables*/
char* menu_content = NULL;

int file_exists(const char* filename) {
    struct stat buffer;
    //return 1 if exists, 0 otherwise
    return (stat(filename, &buffer) == 0);
}

char* generate_unique_filename() {
    static char filename[256];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(filename, sizeof(filename), "%ssystem_%d%d%d_%d%d%d.json",
             DEFAULT_DIRECTORY, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    return filename;
}

FSNode* json_to_fsnode(const cJSON* json_node, FSNode* parent) {
    if (json_node == NULL) {
        printf("Error: Could not reconstruct system from json.\n");
        return NULL;
    }

    //extract node data
    const char* owner = cJSON_GetObjectItem(json_node, "owner")->valuestring;
    const char* name = cJSON_GetObjectItem(json_node, "name")->valuestring;
    const char* permissions_str = cJSON_GetObjectItem(json_node, "permissions")->valuestring;
    const char* type_str = cJSON_GetObjectItem(json_node, "type")->valuestring;

    //determine conditionals
    Permissions permissions =
        (strcmp(permissions_str, "R") == 0) ? Read :
        (strcmp(permissions_str, "RW") == 0) ? Read_Write : Read_Write_Execute;
    NodeType type = (strcmp(type_str, "file") == 0) ? File : Directory;

    //create new node
    FSNode* node = create_node(owner, parent, name, type, permissions);

    //insert node into correct order within parent (skip root: parent == NULL)
    if (parent != NULL) {
        insert_node(parent, node);
    }

    //if copy_node is a file return
    if (node->type == File) {
        return;
    }

    //if node is a directory, retrieve and construct its children
    cJSON* children_array = cJSON_GetObjectItem(json_node, "children");
    if (cJSON_IsArray(children_array)) {
        cJSON* child_json = NULL;
        cJSON_ArrayForEach(child_json, children_array) {
            json_to_fsnode(child_json, node);
        }
    }
    //return root
    return node;
}

void system_load_from_json(FileSystem* system, const char* existing_system) {
    //open system file to read
    FILE* file = fopen(existing_system, "r");
    if (!file) {
        printf("Error: Could not open file for reconstruction/.\n");
        return;
    }

    //get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    //create buffer to read file into a string
    char* buffer = (char*)malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';   //null terminate

    //parse string into json object
    cJSON* json_system = cJSON_Parse(buffer);

    //parse json object and reconstruct system (start with root)
    system->root = json_to_fsnode(json_system, NULL);
    strcpy(system->username, system->root->owner);
    strcpy(system->host_signature, system->username);
    strcat(system->host_signature, "@JDS");
    printf("System successfully reconstructed.\n");

    //clean up
    fclose(file);
    cJSON_Delete(json_system);
    free(buffer);
}

cJSON* fsnode_to_json(const FSNode* node) {
    if (node == NULL) {
        printf("Error: Could not convert system to json object for saving.\n");
        return NULL;
    }

    //create json obj with node data
    cJSON* json_node_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(json_node_obj, "name", node->name);
    cJSON_AddStringToObject(json_node_obj, "owner", node->owner);
    cJSON_AddStringToObject(json_node_obj, "type",node->type == Directory ? "directory" : "file");
    cJSON_AddStringToObject(json_node_obj, "permissions",
        (node->permissions == Read) ? "R" :
             (node->permissions == Read_Write) ? "RW" : "RWE");
    cJSON_AddNumberToObject(json_node_obj, "size", node->size);

    //recursively serialize children of directory
    if (node->type == Directory) {
        cJSON* children_array = cJSON_CreateArray();
        FSNode* iter = node->child_head;

        while (iter != NULL) {
            //create json obj for iter's children
            cJSON_AddItemToArray(children_array, fsnode_to_json(iter));
            iter = iter->next;
        }
        cJSON_AddItemToObject(json_node_obj, "children", children_array);
    }

    return json_node_obj;
}

void system_save_to_json(FileSystem* system, const char* existing_system) {
    //open system file to write
    FILE* file = fopen(existing_system, "w");
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
    FILE* file = fopen("data/menu.txt", "r");

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
    printf("Menu successfully loaded.\n");
}

void free_menu() {
    free(menu_content);
    menu_content = NULL;
}