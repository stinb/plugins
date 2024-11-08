#include <cstdint>

uint16_t func ( uint16_t para1 ) {
   return para1;
}

void discarded ( uint16_t para2 )
{
   func ( para2 );         // UndCC_Violation
   (void)func ( para2 );   // UndCC_Valid
}
