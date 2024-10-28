#define abs1(x) (((x) >= 0) ? (x) : -(x)) // UndCC_Valid
#define abs2(x) ((x >= 0) ? x : -x) // UndCC_Violation
#define subs(x) a ## x // UndCC_Valid
