#include "test_fn_init.h"

#define VAL 0xFA

char init_fn( void ){
  return VAL;
}

char var = init_fn();

//
// test_fn_init
void test_fn_init( test_info_t** info ){
  static test_info_t test_fn_init_info;
  static char* test_fn_init_name = "Initialization of Global by Function";
  test_fn_init_info.name = test_fn_init_name;
  test_fn_init_info.passed = true;
  *(info) = &test_fn_init_info;

  if( var != VAL ){ test_fn_init_info.passed = false; }
}