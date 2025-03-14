#include "process_ui_commands_tests.h"

void setUp(void) {
    // Called before each test
}

void tearDown(void) {
    // Called after each test
}

int main(void) {
    UNITY_BEGIN();

    //run tests
    RUN_TEST(test_validate_args_single_arg_success);
    RUN_TEST(test_validate_args_single_arg_fail);

    return UNITY_END();
}

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