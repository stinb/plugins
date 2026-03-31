// Custom

constexpr int x = 1 - 3;         // UndCC_Valid
const int y = 1 + y;             // UndCC_Valid, y can't be evaluated
const char *s = "Hello";         // UndCC_Violation
const double pi = 3.14;          // UndCC_Violation
