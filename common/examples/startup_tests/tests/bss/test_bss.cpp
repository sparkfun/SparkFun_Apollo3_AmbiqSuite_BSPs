#include "test_bss.h"

#define BYTES_TO_FILL 64

char bss_1[BYTES_TO_FILL];
char bss_2[BYTES_TO_FILL] = {0};

//
// test_bss
void test_bss( test_info_t** info ){
  static test_info_t test_bss_info;
  static char* test_bss_name = "BSS Zereo Fill";
  test_bss_info.name = test_bss_name;
  test_bss_info.passed = true;
  *(info) = &test_bss_info;

  for(size_t ix = 0; ix < BYTES_TO_FILL; ix++){
    if(bss_1[ix] != 0){ test_bss_info.passed = false; }
    if(bss_2[ix] != 0){ test_bss_info.passed = false; }
  }
}