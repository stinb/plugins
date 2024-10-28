#define A(x) #x                       // UndCC_Valid
#define B(x, y) x##y                  // UndCC_Valid
#define C(x, y) #x##y                 // UndCC_Violation
#define D(x2011, y2011) x2011##y2011  // UndCC_Valid
#define E(x2011, y2011) #x2011##y2011 // UndCC_Violation
