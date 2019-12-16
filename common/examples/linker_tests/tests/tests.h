// test definitions
#ifndef _TESTS_H_
#define _TESTS_H_

#include "test_framework.h"

// included tests
#include "test_stack.h"
#include "test_heap.h"

// simple tests
void test_fail( test_info_t** info );
void test_pass( test_info_t** info );

// list of tests to run
extern test_fn tests[];

#endif // _TESTS_H_