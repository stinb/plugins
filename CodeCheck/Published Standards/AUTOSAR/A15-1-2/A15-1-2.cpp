//% $Id: A15-1-2.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
class A
{
  // Implementation
};
void Fn(std::int16_t i)
{
  A a1;
  A& a2 = a1;
  A* a3 = new A;

  if (i < 10)
  {
    throw a1; // UndCC_Valid - copyable object thrown
  }
  else if (i < 20)
  {
    throw A(); // UndCC_Valid - copyable object thrown
  }
  else if (i < 30)
  {
    throw a2; // UndCC_Valid - copyable object thrown
  }
  else if (i < 40)
  {
    throw & a1; // UndCC_Violation - pointer type thrown
  }
  else if (i < 50)
  {
    throw a3; // UndCC_Violation - pointer type thrown
  }
  else if (i < 60)
  {
    throw(*a3); // UndCC_Valid - memory leak occurs, violates other rules
  }
  else
  {
    throw new A; // UndCC_Violation - pointer type thrown
  }

}

void Fn1(std::int16_t i)
{
  std::int16_t j;
  std::int16_t& ptr = j;

  if (i < 10)
  {
    throw ptr; // UndCC_Valid - copyable object thrown
  }
}
