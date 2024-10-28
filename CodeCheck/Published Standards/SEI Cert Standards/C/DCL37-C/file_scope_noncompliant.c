// SEI CERT C++


#include <stddef.h>

static const size_t _max_limit = 1024;  // UndCC_Violation
size_t _limit = 100;  // UndCC_Violation

unsigned int getValue(unsigned int count) {
  return count < _limit ? count : _limit;
}
