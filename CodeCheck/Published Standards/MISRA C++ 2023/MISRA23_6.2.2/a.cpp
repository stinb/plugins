#include <cstdint>

// File a.cpp
typedef int32_t myint;
extern int32_t a;               // UndCC_Violation - see b.cpp
extern int32_t b[];             // UndCC_Valid
extern char c;                  // UndCC_Violation - see b.cpp
extern int32_t d;               // UndCC_Valid
extern myint e;                 // UndCC_Valid
int32_t f1();                   // UndCC_Violation - see b.cpp
int32_t f2(int32_t);            // UndCC_Valid
extern "C" int32_t f3(int32_t); // UndCC_FalseNeg
int32_t f4();                   // UndCC_Violation - see b.cpp
