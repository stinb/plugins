#include <cstdlib>

void f()
{
    int *i = static_cast<int *>(std::malloc(sizeof(int)));
    // ...
    delete i;   // UndCC_Violation
}
