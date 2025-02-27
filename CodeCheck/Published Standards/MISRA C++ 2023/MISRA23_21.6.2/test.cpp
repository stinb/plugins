#include <cstdlib>
#include <iostream>
#include <memory>
#include <memory_resource>

class A
{ /* ... */
};

auto p1 = static_cast<A *>(malloc(sizeof(A))); // UndCC_Violation
auto p2 = new A;                               // UndCC_Violation
auto p3 = std::make_unique<A>();               // UndCC_Valid
auto p4 = p3.release();                        // UndCC_Violation

void f1(std::pmr::memory_resource &mr, A *p)
{
    void *iptr = mr.allocate(sizeof(A)); // UndCC_Violation
    delete p;                            // UndCC_Violation - undefined behaviour if p was allocated using new[]
}