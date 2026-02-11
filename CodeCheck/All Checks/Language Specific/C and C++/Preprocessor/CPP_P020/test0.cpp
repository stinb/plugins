// MISRA C++ 2008

// The following are compliant
#define STOR extern                  // UndCC_Valid - storage class specifier
                                     // Breaks Rule 16–2–1

// The following are non-compliant
#define CLOCK (xtal/16)              // UndCC_Violation - Constant expression
#define PLUS2(X) ((X) + 2)           // UndCC_Violation - Macro expanding to expression
#define PI 3.14159F                  // UndCC_Violation - use const object instead
#define int32_t long                 // UndCC_Violation - use typedef instead
#define STARTIF if(                  // UndCC_Violation - language redefinition
#define INIT(value) {(value), 0, 0}  // UndCC_Violation - braced initializer
#define HEADER "filename.h"          // UndCC_Violation - string literal
