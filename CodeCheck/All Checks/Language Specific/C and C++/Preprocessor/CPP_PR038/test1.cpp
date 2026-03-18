// MISRA C++ 2008

#define abs(x) (((x) >= 0) ? (x) : -(x)) // UndCC_Valid

#define abs(x) ((x >= 0) ? x : -x)       // UndCC_Violation

#define subs(x) a ## x                   // UndCC_Valid
