// The following are compliant
#define STOR extern // storage class specifier
// Breaks Rule 16�2�1
// The following are non-compliant
#define CLOCK (xtal/16) // Constant expression // UndCC_Violation
#define PLUS2(X) ((X) + 2) // Macro expanding to expression // UndCC_Violation
#define PI 3.14159F // use const object instead // UndCC_Violation
#define int32_t long // use typedef instead // UndCC_Violation
#define STARTIF if( // language redefinition // UndCC_Violation
#define INIT(value) {(value), 0, 0} // braced initializer // UndCC_Violation
#define HEADER "filename.h" // string literal // UndCC_Violation
