#define FUNC_MACRO(X) ((X)+(X)) // UndCC_Violation
#define TIMES_TWO(x) x * 2  // UndCC_Violation
#define TIMES_THREE(x) (x) * 3  // UndCC_Violation
#define PLUS1(x) ((x) + 1)  // UndCC_Violation
