#include "test_framework.h"

void print_test_info( test_info_t* info ){
  am_util_stdio_printf("Test Name: %s\n", (info->name != NULL) ? info->name : "N/A");
  am_util_stdio_printf("==========\n");
  am_util_stdio_printf("status: %s\n", (info->passed) ? "passed" : "failed" );
  am_util_stdio_printf("\n");
}
void run_tests(test_fn* tests){
  test_info_t* info;
  while(*tests != NULL){
    (*tests)(&info);
    print_test_info(info);
    tests++;
  }
  am_util_stdio_printf("\n\nAll tests complete\n");
}