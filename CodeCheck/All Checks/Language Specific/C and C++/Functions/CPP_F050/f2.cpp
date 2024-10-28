//% $Id: A15-4-3.cpp 317753 2018-04-27 07:44:02Z jan.babst $
// f2.cpp

#include "f1.hpp"
#ifdef _WIN32
void Fn() noexcept(false) // UndCC_Violation(win) - different exception specifier
{
  // Implementation
}
#endif

class A
{
  public:
    void F() noexcept;
    void G() noexcept(false);
    virtual void V1() noexcept = 0;
    virtual void V2() noexcept(false) = 0;
};

void A::F() noexcept // UndCC_Valid
   
{
  // Implementation
}

void A::G() noexcept(false) // UndCC_Valid
  
{
  // Implementation
}

class B : public A
{
  public:
    void V1() noexcept override // UndCC_Valid
     
    // exception specifier in derived method, non-compilable
    {
      // Implementation
    }
    void V2() noexcept override // UndCC_Valid - stricter noexcept specification
    {
      // Implementation
    }
};
