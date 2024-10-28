#include "A.h" // This header defines the macro M
#undef M       // UndCC_Violation - defined in another file
#define ID(name) constexpr auto name = #name
ID(IdA);
ID(IdB);
#undef ID // UndCC_Valid - defined in this file
