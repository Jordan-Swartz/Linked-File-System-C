cmake_minimum_required(VERSION 3.29)
project(LinkedFS C)

set(CMAKE_C_STANDARD 99)

#header files
include_directories(include)

#source files
#file(GLOB SOURCES "src/*.c")
set(SOURCES
        src/ui.c
        src/file_system.c
        src/file_operation.c
        src/memory_management.c
        src/stack.c
)

add_executable(LinkedFS ${SOURCES})