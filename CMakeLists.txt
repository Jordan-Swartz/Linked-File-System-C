cmake_minimum_required(VERSION 3.29)
project(LinkedFS C)

set(CMAKE_C_STANDARD 99)

#header files
include_directories(include)

#source files
file(GLOB SOURCES "src/*.c")

add_executable(LinkedFS ${SOURCES}
        include/file_system.h
        include/file_operation.h
        include/memory_management.h
        include/ui.h
        src/file_system.c
        src/memory_management.c
        src/ui.c
        include/ui.h)
