#include "tests.h"

test_fn tests[] = {
  test_data,
  test_bss,
  test_constructors,
  test_fn_init,

  // test_fail,
  // test_pass,

  NULL, // NULL terminates the list
};

// test definitions
void test_fail( test_info_t** info ){
  static test_info_t test_fail_info;
  static char* test_fail_name = "Fail Test";
  test_fail_info.name = test_fail_name;
  test_fail_info.passed = false;
  *(info) = &test_fail_info;
}

void test_pass( test_info_t** info ){
  static test_info_t test_pass_info;
  static char* test_pass_name = "Pass Test";
  test_pass_info.name = test_pass_name;
  test_pass_info.passed = true;
  *(info) = &test_pass_info;
}