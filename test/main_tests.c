#include "main_tests.h"

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    //file_system_tests
    RUN_TEST(test_system_setup);
    RUN_TEST(test_root_setup);
    RUN_TEST(test_create_node);
    RUN_TEST(test_create_copy_node);
    RUN_TEST(test_insert_node);
    RUN_TEST(test_recursive_copy);
    RUN_TEST(test_recursive_delete);
    RUN_TEST(test_change_directory_forward);
    RUN_TEST(test_is_subdirectory);
    RUN_TEST(test_find_node);


    //ui_tests
    RUN_TEST(test_validate_args_single_arg_success);
    RUN_TEST(test_validate_args_single_arg_fail);
    RUN_TEST(test_process_mkdir);
    RUN_TEST(test_process_touch);
    RUN_TEST(test_process_cd);
    RUN_TEST(test_process_mv);
    RUN_TEST(test_process_chmod);

    return UNITY_END();
}