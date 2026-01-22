// Custom

#define SUM(a, b, c) ((a) + (b) + (c))     // UndCC_Violation

#define PRODUCT(a, b, c) ((a) * (b) * (c)) // UndCC_Valid
#define RESULT PRODUCT(2, 4, 6)            // UndCC_Violation
