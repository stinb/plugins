#include <cstdint>

class C
{
public:
    int32_t b; // UndCC_Violation
    int foo();

protected:
    int32_t c; // UndCC_Violation
private:
    int32_t d; // UndCC_Valid
};