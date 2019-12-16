#include "tests.h"

test_fn tests[] = {
  test_stack,
  test_heap,

  // test_fail,
  // test_pass,

  NULL, // NULL terminates the list
};

// test definitions
void test_fail( test_info_t** info ){
  static test_info_t test_fail_info;
  static char* test_fail_name = "Fail Test";
  test_fail_info.name = test_fail_name;
  test_fail_info.metric = "success";
  test_fail_info.value = 0;
  *(info) = &test_fail_info;
}

void test_pass( test_info_t** info ){
  static test_info_t test_pass_info;
  static char* test_pass_name = "Pass Test";
  test_pass_info.name = test_pass_name;
  test_pass_info.metric = "success";
  test_pass_info.value = 1;
  *(info) = &test_pass_info;
}