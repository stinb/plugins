#include <iostream>
using namespace std;

// Declare Base Class
class Base
{
public:
  std::string str;
};

// Declare Derived Class
class Derived : public Base
{
public:
  int b;

  Derived(int n)
  try : b(n)
  {
    if (n)
    {
      // ...
    }
  }
  catch (...)
  {
    // ...
  }

  ~Derived()
  try
  {
    if (!str.empty()) // UndCC_Violation
    {
      // ...
    }
  }
  catch (...)
  {
    // ...
  };
};

