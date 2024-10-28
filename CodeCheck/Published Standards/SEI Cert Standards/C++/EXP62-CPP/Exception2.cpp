// Custom

#include <cstring>

class C
{
    int i : 10;
    int j;
};

void f(const C &c1)
{
    C c2;
    std::memcpy(&c2, &c1, sizeof(C));
}
