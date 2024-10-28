#include <iostream>

#define LOG printf

class foo {
  foo& operator=(const foo& right);
};

class bar {
  bar& operator=(const bar& right);
};

class foobar {
  foobar& operator=(const foobar& right);
};

foo& foo::operator=(const foo& right)
{
  LOG("Method Entry" // UndCC_Violation
      "on two lines");
  if (this != &right)
  {
  }
  LOG("Method Exit");
  return *this;
}

bar& bar::operator=(const bar& right)
{
  LOG("Method Entry/Exit with nested parens",(1),((1))); // UndCC_Violation
  if (this != &right)
  {
  }
  return *this;
}

foobar& foobar::operator=(const foobar& right)
{
  LOG("Method Entry/Exit"); // UndCC_Violation
  if (this != &right)
  {
    LOG("Inside self-assignment check");
  }
  return *this;
}
