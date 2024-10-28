#define A(x) #x                // UndCC_Valid
#define B(x, y) x##y           // UndCC_Valid
#define C(x, y) #x##y          // UndCC_Violation
#define D(x, y, z, yz) x##y##z // UndCC_Violation
#define E(x, y) #x #y          // UndCC_Violation
#define F(x) x                 // UndCC_Valid