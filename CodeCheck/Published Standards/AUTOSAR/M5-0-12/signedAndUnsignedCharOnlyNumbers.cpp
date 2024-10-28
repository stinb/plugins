#include <cstdint>

     int8_t a = 'a';  // UndCC_Violation - explicitly signed
    uint8_t b = '\r'; // UndCC_Violation - explicitly unsigned
     int8_t c = 10;   // UndCC_Valid
    uint8_t d = 12U;  // UndCC_Valid
signed char e = 11;   // UndCC_Valid with this rule, but breaks Rule 3-9-2
