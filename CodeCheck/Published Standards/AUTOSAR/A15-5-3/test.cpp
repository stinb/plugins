//% $Id: A15-5-3.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <stdexcept>
#include <thread>
#include <exception>

extern bool F1();
class A
{
  public:
    A() noexcept(false)
    {
      // ...
      throw std::runtime_error("Error1");
    }
    ~A()
    {
      // ...
      throw std::runtime_error("Error2");
      
    }
};

class B
{
  public:
    ~B() noexcept(false)
    {
      // ...
      throw std::runtime_error("Error3");
    }
};

void F2()
{
  throw;
}

void ThreadFunc()
{
  A a;
}

void F3() {
  try
  {
    std::terminate(); // UndCC_Violation - terminate called
    std::thread t(&ThreadFunc);
    
    if (F1())
    {
      throw std::logic_error("Error4");
    }
    else
    {
      F2();

    }
  }
  catch (...)
  {
    B b;  
    // ...
    F2();
  }
}
static A a;  
int main(int, char**)
{
  F3();  
  return 0;
}
