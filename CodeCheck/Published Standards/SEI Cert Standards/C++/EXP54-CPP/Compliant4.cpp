#include <iostream>
#include <vector>

class C
{
    std::vector<int> l;

public:
    C() : l{1, 2, 3} {}

    int first() const { return *l.begin(); }
};

void f()
{
    C c;
    std::cout << c.first();
}