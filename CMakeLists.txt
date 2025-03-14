cmake_minimum_required(VERSION 3.29)
project(LinkedFS C)

set(CMAKE_C_STANDARD 99)

#header files
include_directories(include)
include_directories(lib)
include_directories(lib/unity)

#source files
set(SOURCES
        src/ui/ui.c
        src/util/file_system.c
        src/util/file_operation.c
        src/util/memory_management.c
        src/util/stack.c
        lib/cJSON/cJSON.c
        src/ui/main_program.c
)

#main executable
add_executable(LinkedFS ${SOURCES})

#test executable
add_executable(test_ui
        test/ui_tests.c
        src/ui/ui.c
        src/util/file_system.c
        src/util/file_operation.c
        src/util/memory_management.c
        src/util/stack.c
        lib/cJSON/cJSON.c
        test/main_tests.c
        include/main_tests.h
        test/file_system_tests.c
        include/file_system_tests.h
)
target_link_libraries(test_ui unity)

#define TEST_UI to prevent main from being included in the tests
target_compile_definitions(test_ui PRIVATE TEST_UI)

#include unity config
include(${CMAKE_CURRENT_LIST_DIR}/lib/unity/unityConfig.cmake)

#add data directory in build path
add_custom_target(copy_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
)
add_dependencies(LinkedFS copy_data)
add_dependencies(test_ui copy_data)