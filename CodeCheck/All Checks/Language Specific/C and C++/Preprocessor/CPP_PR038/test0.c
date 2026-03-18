// MISRA C 2004

#define abs(x) (((x) >= 0) ? (x) : -(x)) // UndCC_Valid

#define abs(x) ((x >= 0) ? x : -x)       // UndCC_Violation
