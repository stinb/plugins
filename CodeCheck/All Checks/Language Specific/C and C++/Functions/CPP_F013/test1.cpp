// MISRA C++ 2008

#include <stdint.h>

class C
{
public:
   C ( int32_t * shared ) : m_shared ( shared )
   {
   }

   int32_t * getA ()
   {
      return m_shared;   // UndCC_Valid - m_shared is not class-data
   }

private:
   int32_t * m_shared;
};
