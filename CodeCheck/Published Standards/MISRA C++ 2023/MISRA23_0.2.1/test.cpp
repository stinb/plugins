#include <cstdint>
#include <cassert>
#include <fstream>

void usefn(int32_t);

class C
{
}; // No user-provided constructor or destructor
namespace
{
    C c; // UndCC_Violation - unused
}
void maybeUnused(int32_t a)
{
    [[maybe_unused]] bool b = a > 0; // UndCC_Valid (by exception #1 if NDEBUG is defined)
    assert(b);                       // Does not use b if NDEBUG is defined
    usefn(a);
}
const int16_t x = 19; // UndCC_Valid - x is read in initializedButNotUsed
const int16_t y = 21; // UndCC_Violation - would be compliant by exception #2
                      // if declared in a header file
void initializedButNotUsed()
{
    int16_t local_1 = 42; // UndCC_Violation - local_1 is never read
    int16_t local_2;      // UndCC_Valid
    local_2 = x;          // Use of local_2 for the purposes of this rule
}

void userProvidedCtor()
{
    std::ifstream fs{"cfg.ini"}; // UndCC_Valid - user-provided constructor
}
