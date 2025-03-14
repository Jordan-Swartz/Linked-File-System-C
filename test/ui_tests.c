#include "ui_tests.h"

void test_validate_args_single_arg_success(void) {
    int result = validate_args("mkdir", "dir1", NULL, SINGLE_ARG);
    TEST_ASSERT_EQUAL_INT(Success, result);
}

void test_validate_args_single_arg_fail(void) {
    //too many args
    int result = validate_args("mkdir", "dir1", "dir2", SINGLE_ARG);
    TEST_ASSERT_EQUAL_INT(Error, result);

    //missing args
    result = validate_args("mkdir", NULL, NULL, SINGLE_ARG);
    TEST_ASSERT_EQUAL_INT(Error, result);
}

void test_process_mkdir(void) {
    FileSystem system;
    system.root = create_node("user", NULL, "root", Directory, Read_Write);
    FSNode* current = system.root;

    process_mkdir(&system, "mkdir", "new_dir", NULL, current);
    FSNode* created = find_node(current, "new_dir");

    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_EQUAL_STRING("new_dir", created->name);
}

void test_process_touch(void) {
    FileSystem system;
    system.root = create_node("user", NULL, "root", Directory, Read_Write);
    FSNode* current = system.root;

    process_touch(&system, "touch", "/", "new_file", current);
    FSNode* created = find_node(current, "new_file");

    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_EQUAL_STRING("new_file", created->name);
    TEST_ASSERT_EQUAL(File, created->type);
}

void test_process_cd(void) {
    FileSystem system;
    system.root = create_node("user", NULL, "root", Directory, Read_Write);
    FSNode* child = create_node("user", system.root, "child", Directory, Read_Write);
    insert_node(system.root, child);

    FSNode* current = system.root;
    process_cd(&system, "cd", "child", NULL, &current);

    TEST_ASSERT_EQUAL(child, current);
}

void test_process_mv(void) {
    FileSystem system;
    FSNode root = {.name = "root", .type = Directory, .parent = NULL, .child_head = NULL};
    system.root = &root;

    FSNode* current = &root;
    FSNode* file = create_node("user", &root, "file1.txt", File, Read_Write);
    FSNode* dest = create_node("user", &root, "dir1", Directory, Read_Write);
    insert_node(&root, file);
    insert_node(&root, dest);

    process_mv(&system, "mv", "file1.txt", "dir1", current);

    TEST_ASSERT_NULL(find_node(&root, "file1.txt")); //assert source removed from root
    TEST_ASSERT_NOT_NULL(find_node(dest, "file1.txt")); //assert file moved to dir1
}

void test_process_chmod(void) {
    FileSystem system;
    FSNode root = {.name = "root", .type = Directory, .parent = NULL, .child_head = NULL};
    system.root = &root;

    FSNode* file = create_node("user", &root, "file1.txt", File, Read_Write);
    insert_node(&root, file);

    process_chmod(&system, "chmod", "file1.txt", "R", &root);

    TEST_ASSERT_EQUAL(Read, file->permissions); //assert permissions changed to read
}

