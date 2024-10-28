#include <iostream>

struct S
{
    S() { std::cout << "S::S()" << std::endl; }
    ~S() { std::cout << "S::~S()" << std::endl; }
};

void f()
{
    alignas(struct S) char space[sizeof(struct S)];
    S *s1 = new (&space) S;

    // ...

    delete s1;  // UndCC_Violation
}
