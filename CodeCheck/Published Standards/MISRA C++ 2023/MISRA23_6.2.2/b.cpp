#include <cstdint>

// File b.cpp
extern int64_t a;            // UndCC_Violation - see a.cpp
extern int32_t b[5];         // UndCC_Valid
int16_t c = 1;               // UndCC_Violation - see a.cpp
int32_t d{1};                // UndCC_Valid
int32_t e;                   // UndCC_Valid
char f1();                   // UndCC_Violation - see a.cpp
char f2(char);               // UndCC_Valid - not the same function as
                             // int32_t f2( int32_t )
extern "C" int32_t f3(char); // UndCC_FalseNeg
int32_t f4() noexcept;       // UndCC_Violation - see a.cpp
                             // Different exception specification
