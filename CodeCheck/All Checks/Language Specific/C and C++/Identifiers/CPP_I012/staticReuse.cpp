#include <cstdint>
namespace NS1 
{ 
  static int32_t global = 0; 
}

namespace NS2 
{ 
void fn ( ) 
{ 
  int32_t global;      // UndCC_Violation 
} 
}

