//
// Created by jordan on 10/26/24.
//

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

/*Global Variables*/
extern char* menu_content;

void system_load(FileSystem* system);
void system_save(FileSystem* system);
void load_menu();
void free_menu();

#endif //FILE_OPERATION_H

