#ifndef _TEST_FRAMEWORK_H_
#define _TEST_FRAMEWORK_H_

#include "main.h"
#include <stdlib.h>

typedef struct _test_info_t {
  char* name;
  bool passed;
} test_info_t;
typedef void (*test_fn)( test_info_t** info );
void print_test_info( test_info_t* info );
void run_tests(test_fn* tests);

#endif // _TEST_FRAMEWORK_H_