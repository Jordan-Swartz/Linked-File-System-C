//
// Created by jordan on 10/26/24.
//

#ifndef UI_H
#define UI_H

#include "file_system.h"
#include "memory_management.h"

//traversal options
#define STOP_AT_LAST 1
#define FULL_TRAVERSAL 0

//node creation options
#define ENABLE_CREATE 1
#define DISABLE_CREATE 0

//return status
typedef enum { Success, Error, Error_File, Exit } InputReturn;

void display_menu();
int process_input_command_2();
int process_input_command(const FileSystem* system, FSNode** current);
int validate_args();
int process_parsed_path();
void process_mkdir();
void process_touch();
void process_rm();
void process_pwd();
void process_cd();



#endif //UI_H
