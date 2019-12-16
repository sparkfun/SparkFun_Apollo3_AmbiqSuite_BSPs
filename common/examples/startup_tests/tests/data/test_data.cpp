#include "test_data.h"

#define BYTES_TO_COPY 8

// Automatially add some variables that won't be optimized
// away to fill up the number of requested bytes
#ifdef BYTES_TO_COPY
#if BYTES_TO_COPY > 0
#define VAR11 0xDE
#endif
#if BYTES_TO_COPY > 1
#define VAR12 0xAD
#endif
#if BYTES_TO_COPY > 2
#define VAR13 0xBE
#endif
#if BYTES_TO_COPY > 3
#define VAR14 0xEF
#endif
#if BYTES_TO_COPY > 4
#define VAR21 0xC0
#endif
#if BYTES_TO_COPY > 5
#define VAR22 0xFE
#endif
#if BYTES_TO_COPY > 6
#define VAR23 0xE6
#endif
#if BYTES_TO_COPY > 7
#define VAR24 0x0D
#endif
#endif // BYTES_TO_COPY

#ifdef BYTES_TO_COPY
static volatile char data[] = {
#if BYTES_TO_COPY > 0
VAR11,
#endif
#if BYTES_TO_COPY > 1
VAR12,
#endif
#if BYTES_TO_COPY > 2
VAR13,
#endif
#if BYTES_TO_COPY > 3
VAR14,
#endif
#if BYTES_TO_COPY > 4
VAR21,
#endif
#if BYTES_TO_COPY > 5
VAR22,
#endif
#if BYTES_TO_COPY > 6
VAR23,
#endif
#if BYTES_TO_COPY > 7
VAR24,
#endif
};
#endif // BYTES_TO_COPY

//
// test_data
void test_data( test_info_t** info ){
  static test_info_t test_data_info;
  static char* test_data_name = "Data Segment Copy";
  test_data_info.name = test_data_name;
  test_data_info.passed = true;
  *(info) = &test_data_info;

  // Verify that all bytes were copied into RAM correctly
#ifdef BYTES_TO_COPY
#if BYTES_TO_COPY > 0
  if( data[0] != VAR11 ){ test_data_info.passed = false; }
#endif
#if BYTES_TO_COPY > 1
  if( data[1] != VAR12 ){ test_data_info.passed = false; }
#endif
#if BYTES_TO_COPY > 2
  if( data[2] != VAR13 ){ test_data_info.passed = false; }
#endif
#if BYTES_TO_COPY > 3
  if( data[3] != VAR14 ){ test_data_info.passed = false; }
#endif
#if BYTES_TO_COPY > 4
  if( data[4] != VAR21 ){ test_data_info.passed = false; }
#endif
#if BYTES_TO_COPY > 5
  if( data[5] != VAR22 ){ test_data_info.passed = false; }
#endif
#if BYTES_TO_COPY > 6
  if( data[6] != VAR23 ){ test_data_info.passed = false; }
#endif
#if BYTES_TO_COPY > 7
  if( data[7] != VAR24 ){ test_data_info.passed = false; }
#endif
#endif // BYTES_TO_COPY
}