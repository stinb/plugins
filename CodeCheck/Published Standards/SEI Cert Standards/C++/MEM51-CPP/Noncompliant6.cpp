#include <cstdlib>

struct S
{
    ~S();
};

void f()
{
    S *s = new S();
    // ...
    std::free(s);       // UndCC_Violation
}
