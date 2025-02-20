# Define Unity as a static library with the source and header files
add_library(unity STATIC
        ${CMAKE_CURRENT_LIST_DIR}/unity.c   # Unity source file
        ${CMAKE_CURRENT_LIST_DIR}/unity.h   # Unity header file
)

# Make sure the include directories are available to other targets that link to unity
target_include_directories(unity PUBLIC ${CMAKE_CURRENT_LIST_DIR})
