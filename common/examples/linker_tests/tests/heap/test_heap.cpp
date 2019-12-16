#include "test_heap.h"

//
// test_heap
void test_heap( test_info_t** info ){
  static test_info_t test_heap_info;
  static char* test_heap_name = "Heap Allocation";
  test_heap_info.name = test_heap_name;
  *(info) = &test_heap_info;

  void* mem = NULL;
  size_t len = 0;
  // boost_mode_enable(true);
  do {
    len++;
    mem = (void*)malloc( len * sizeof(uint8_t));
    free(mem);
  } while (mem != NULL);
  // boost_mode_enable(false); 

  test_heap_info.metric = "largest allocated space";
  test_heap_info.value = len;
}