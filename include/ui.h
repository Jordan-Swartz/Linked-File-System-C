//
// Created by jordan on 10/26/24.
//

#ifndef UI_H
#define UI_H

#include "file_system.h"
#include "memory_management.h"

//return status
typedef enum { Success, Error, Error_File, Exit } InputReturn;

void display_menu();
int process_input_command();
int process_input_command(const FileSystem* system, FSNode** current);
char** parse_path();


#endif //UI_H
