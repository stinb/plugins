#include <initializer_list>
#include <cstddef>

class A // UndCC_Violation
{
public:
    A(std::size_t x, std::size_t y);
    A(std::initializer_list<std::size_t> list);
};
class B // UndCC_Valid - no initializer-list constructor
{
public:
    B(std::size_t x, std::size_t y);
    // The following is not an initializer-list constructor
    B(std::size_t x, std::initializer_list<std::size_t> list);
};
class C // UndCC_Valid
{
public:
    C(std::initializer_list<std::size_t> list);
};
class D // UndCC_Valid
{
public:
    D(const D &d);
    D(D &&d);
    D(std::initializer_list<std::size_t> list, int x = 4);
};
