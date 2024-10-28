//% $Id: A3-3-1.hpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>

extern std::int32_t a1;
extern void F4();

namespace n
{
  void F2();
  std::int32_t a5; // UndCC_Valid, external linkage
}

int F6( int a );
