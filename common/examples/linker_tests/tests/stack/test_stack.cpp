#include "test_stack.h"


#define MEMORY_HEADSPACE 4096

// Globals
uint32_t stack_pointer;
uint32_t min_stack_pointer = 0xFFFFFFFF;
uint32_t free_mem;
uint32_t min_free_mem = 0xFFFFFFFF;
bool go_deeper = true;
uint32_t max_depth = 0;

extern unsigned char _sheap;
uint32_t free_memory( void ){
    // Without an implementation of _sbrk (heap management) we are assuming 
    // that the heap has zero size. If there was heap management then you 
    // would compute the distance to the program break (the end of the heap)
    void* local;
    return (((uint32_t)&local) - ((uint32_t)&_sheap));
}

void update_stack_info( void ){
    void* local;
    stack_pointer = (uint32_t)(&local);
    free_mem = free_memory();
    min_free_mem = (free_mem < min_free_mem) ? free_mem : min_free_mem;
    min_stack_pointer = (stack_pointer < min_stack_pointer) ? stack_pointer : min_stack_pointer;
}

void deep_horizon( void ){
    update_stack_info();
    if(free_memory() < MEMORY_HEADSPACE){
        go_deeper = false;
        return;
    }
    if( go_deeper ){
        deep_horizon();
    }
    max_depth++;
}

//
// test_stack
void test_stack( test_info_t** info ){
  static test_info_t test_stack_info;
  static char* test_stack_name = "Stack Allocation";
  test_stack_info.name = test_stack_name;
  *(info) = &test_stack_info;

  go_deeper = true;
  min_free_mem = 0xFFFFFFFF;
  max_depth = 0;
  deep_horizon();
  
  test_stack_info.metric = "recursion depth";
  test_stack_info.value = max_depth;
}