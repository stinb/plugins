// MISRA C++ 2008

#include <cstdint>

using float32_t = float;

namespace NS1
{
                // UndCC_Valid - no union
}

namespace NS2
{
   union U1     // UndCC_Violation - union
   {
      int32_t   i;
      float32_t j;
   };
}
