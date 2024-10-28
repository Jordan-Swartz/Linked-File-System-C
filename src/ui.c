/**
 * user interface:
 *
 * prompt for commands
 * display outputs
 *
 */

#include <stdlib.h>		//standard lib
#include <stdio.h>		//I/O features
#include <string.h>		//String lib

//header files
#include "ui.h"
#include "file_system.h"
#include "file_operation.h"

int main(void) {
    system_load();

    system_save();
    return 0;
}

void system_load() {
    //open system file to read
   FILE* file = fopen("", "r");
}

void system_save() {
    //open system file to write
   FILE* file = fopen("", "w");
}


