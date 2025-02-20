cmake_minimum_required(VERSION 3.29)
project(LinkedFS C)

set(CMAKE_C_STANDARD 99)

#header files
include_directories(include)
include_directories(lib)

#source files
#file(GLOB SOURCES "src/*.c")
set(SOURCES
        src/ui/ui.c
        src/util/file_system.c
        src/util/file_operation.c
        src/util/memory_management.c
        src/stack.c
        lib/cJSON/cJSON.c
)

add_executable(LinkedFS ${SOURCES})