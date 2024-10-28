// $Id: A5-2-4.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
#include <string>
void F1() noexcept
{
    std::string str = "Hello";
    std::int32_t* ptr = reinterpret_cast<std::int32_t*>(&str);  // UndCC_Violation
}
struct A
{
    std::int32_t x;
    std::int32_t y;
};
class B
{
  public:
    virtual ~B() {}

  private:
    std::int32_t x;
};
class C : public B
{
};
class D : public B
{
};
void F2(A* ptr) noexcept
{
    B* b1 = reinterpret_cast<B*>(ptr);  // UndCC_Violation
    std::int32_t num = 0;
    A* a1 = reinterpret_cast<A*>(num);  // UndCC_Violation
    A* a2 = (A*)
        num;  // UndCC_Valid with this rule, but non-compliant with Rule A5-2-2.
    B* b2 = reinterpret_cast<B*>(num);  // UndCC_Violation
    D d;
    C* c1 = reinterpret_cast<C*>(&d);  // UndCC_Violation - cross cast
    C* c2 = (C*)&d;  // UndCC_Valid with this rule, but non-compliant with Rule
                     // A5-2-2. Cross-cast.
    B* b3 = &d;      // UndCC_Valid - class D is a subclass of class B
}
