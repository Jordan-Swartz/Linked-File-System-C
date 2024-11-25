//
// Created by jordan on 10/26/24.
//

#ifndef UI_H
#define UI_H

#include "file_system.h"

//return status
typedef enum { Success, Error, Exit } InputReturn;

void display_menu();
int process_input_command(const FileSystem* system, FSNode** current);
void parse_relative_path();

#endif //UI_H
