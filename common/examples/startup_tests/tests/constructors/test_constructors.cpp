#include "test_constructors.h"

#define STATE1 0x00
#define STATE2 0x01
#define STATE3 0xDD
#define STATE4 0xCC

/* A very simple class to test constructor execution */
class LEDSTATE {
private:
protected:
public:
    uint8_t state;          // Trivial initialization is 0
    LEDSTATE(uint8_t init);
};

LEDSTATE::LEDSTATE(uint8_t init){
    state=init;
}

// This object should be initialized with a non-zero state
LEDSTATE    state1(STATE1);
LEDSTATE    state2(STATE2);
LEDSTATE    state3(STATE3);

//
// test_constructors
void test_constructors( test_info_t** info ){
  static LEDSTATE    state4(STATE4);

  static test_info_t test_constructors_info;
  static char* test_constructors_name = "Global / Static Constructors";
  test_constructors_info.name = test_constructors_name;
  test_constructors_info.passed = true;
  *(info) = &test_constructors_info;

  if(state1.state != STATE1){ test_constructors_info.passed = false; }
  if(state2.state != STATE2){ test_constructors_info.passed = false; }
  if(state3.state != STATE3){ test_constructors_info.passed = false; }
  if(state4.state != STATE4){ test_constructors_info.passed = false; }
}