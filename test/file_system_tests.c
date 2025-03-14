#include "file_system_tests.h"

void test_system_setup(void) {
    FileSystem system;
    strcpy(system.username, "");

    //system attribute setup
    strcpy(system.username, "testuser");
    strcpy(system.host_signature, "testuser@JDS");

    TEST_ASSERT_EQUAL_STRING("testuser", system.username);
    TEST_ASSERT_EQUAL_STRING("testuser@JDS", system.host_signature);
}

void test_root_setup(void) {
    FileSystem system;
    strcpy(system.username, "testuser");
    FSNode root;

    //root attribute setup
    strcpy(root.name, "");
    root_setup(&system, &root);

    TEST_ASSERT_EQUAL_STRING("testuser", root.owner);
    TEST_ASSERT_EQUAL(Directory, root.type);
    TEST_ASSERT_EQUAL(Read_Write, root.permissions);
}

void test_create_node(void) {
    FSNode parent;
    FSNode* node = create_node("user", &parent, "node1", File, Read_Write);

    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_STRING("node1", node->name);
    TEST_ASSERT_EQUAL(File, node->type);
    TEST_ASSERT_EQUAL(Read_Write, node->permissions);

    free(node);
}

void test_create_copy_node(void) {
    FSNode original;
    strcpy(original.name, "original");
    strcpy(original.owner, "user");
    original.type = File;

    FSNode* copy = create_copy_node(&original, NULL);

    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_STRING("original", copy->name);
    TEST_ASSERT_EQUAL_STRING("user", copy->owner);
    TEST_ASSERT_EQUAL(File, copy->type);

    free(copy);
}

void test_insert_node(void) {
    FSNode parent = {.child_head = NULL, .size = 0};

    //insert nodes in non-alphabetical order
    FSNode* node_b = create_node("user", &parent, "b", File, Read);
    FSNode* node_a = create_node("user", &parent, "a", File, Read);
    FSNode* node_c = create_node("user", &parent, "c", File, Read);
    FSNode* duplicate_b = create_node("user", &parent, "b", File, Read);

    insert_node(&parent, node_b);
    insert_node(&parent, node_a);
    insert_node(&parent, node_c);
    int duplicate_result = insert_node(&parent, duplicate_b);

    //assert that node order is sorted as a->b->c
    TEST_ASSERT_NOT_NULL(parent.child_head);
    TEST_ASSERT_EQUAL_STRING("a", parent.child_head->name);
    TEST_ASSERT_NOT_NULL(parent.child_head->next);
    TEST_ASSERT_EQUAL_STRING("b", parent.child_head->next->name);
    TEST_ASSERT_NOT_NULL(parent.child_head->next->next);
    TEST_ASSERT_EQUAL_STRING("c", parent.child_head->next->next->name);
    TEST_ASSERT_NULL(parent.child_head->next->next->next);

    //assert duplicate insertion failed
    TEST_ASSERT_EQUAL(Error, duplicate_result);
}

void test_recursive_copy(void) {
    FSNode source;
    FSNode destination;
    strcpy(source.name, "src");
    source.type = Directory;
    source.child_head = create_node("user", &source, "child", File, Read);

    destination.child_head = NULL;

    recursive_copy(&source, &destination);

    TEST_ASSERT_NOT_NULL(destination.child_head);
    TEST_ASSERT_EQUAL_STRING("src", destination.child_head->name);
    TEST_ASSERT_NOT_NULL(destination.child_head->child_head);
    TEST_ASSERT_EQUAL_STRING("child", destination.child_head->child_head->name);
}

void test_recursive_delete(void) {
    FSNode* node = create_node("user", NULL, "delete_me", Directory, Read);
    node->child_head = create_node("user", node, "child", File, Read);

    recursive_delete(node);
    //if program does not crash from recursion then method passes
    TEST_PASS();
}

void test_change_directory_forward(void) {
    FSNode root = {.child_head = create_node("user", NULL, "dir", Directory, Read)};
    FSNode* current = &root;

    int result = change_directory_forward(&current, "dir");

    TEST_ASSERT_EQUAL(Success, result);
    TEST_ASSERT_EQUAL_STRING("dir", current->name);
}

void test_is_subdirectory(void) {
    FSNode parent = {.name = "parent", .parent = NULL};
    FSNode child = {.name = "child", .parent = &parent};

    int result = is_subdirectory(&parent, &child);

    TEST_ASSERT_EQUAL(Error, result);
}

void test_find_node(void) {
    FSNode root = {.child_head = create_node("user", NULL, "findme", File, Read)};

    FSNode* found = find_node(&root, "findme");

    TEST_ASSERT_NOT_NULL(found);
    TEST_ASSERT_EQUAL_STRING("findme", found->name);
}