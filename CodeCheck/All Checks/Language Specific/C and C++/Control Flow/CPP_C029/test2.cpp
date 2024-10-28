// Custom


#include <vector>


// Function try blocks

void fn0() try  // UndCC_Valid - 1 implicit return at each end
{

}
catch (...)
{

}

void fn1() try  // UndCC_Valid - 1 return at each end
{
  return;
}
catch (...)
{
  return;
}

void fn2() try  // UndCC_Violation(decl) - 1 early return
{
  return;
}
catch (...)
{
  if (1)
    return;     // UndCC_Violation(exit) - early return
  return;
}


// Return objects with { }

std::vector<int> fn3()  // UndCC_Valid - 1 return at end
{
  return { 0, 1 };
}

std::vector<int> fn4()  // UndCC_Violation(decl) - 1 early return
{
  if (1)
    return { 1 };       // UndCC_Violation(exit) - early return
  return { 0 };
}


// Lambdas

int fn5()          // UndCC_Valid - 1 return at end
{
  return []() {    // UndCC_Valid - 1 return at end
    return 1;
  }();
}

int fn6()          // UndCC_Violation(decl) - 1 early return
{
  if (1)
    return []() {  // UndCC_Violation(exit) - early return
      return 1;
    }();

  return []() {    // UndCC_Valid - 1 return at end
    return 0;
  }();
}

int fn7()          // UndCC_Valid - 1 return at end
{
  return []() {    // UndCC_Violation(decl) - 1 early return
    if (1)
      return 1;    // UndCC_Violation(exit) - early return
    return 0;
  }();
}


// Position of one return

void fn8()                             // UndCC_Violation(decl) - 1 early return
{
  if (1)
    return;                            // UndCC_Violation(exit) - early return
}

void fn9()                             // UndCC_Violation(decl) - 1 early return
{
  while (1)
    return;                            // UndCC_Violation(exit) - early return
}

void fn10()                            // UndCC_Violation(decl) - 1 early return
{
  for (;;)
    return;                            // UndCC_Violation(exit) - early return
}

void fn11()                            // UndCC_Violation(decl) - 1 early return
{
  do
    return;                            // UndCC_Violation(exit) - early return
  while (1);
}

void fn12()                            // UndCC_Violation(decl) - 1 early return
{
  int x = 0;
  switch (x)
    return;                            // UndCC_Violation(exit) - early return
}

void fn13()                            // UndCC_Violation(decl) - 1 early return
{
  int x = 0;
  switch (x) {
    case 0:
      return;                          // UndCC_Violation(exit) - early return
  }
}

void fn14()                            // UndCC_Violation(decl) - 1 early return
{
  int x = 0;
  switch (x) {
    default:
      return;                          // UndCC_Violation(exit) - early return
  }
}

void fn15()                            // UndCC_Violation(decl) - 1 early return
{
  try {
    return;                            // UndCC_Violation(exit) - early return
  } catch (...) {

  }
}

void fn16(std::vector<int> nums = {})  // UndCC_Violation(decl) - 1 early return
{
  try {

  } catch (...) {
    return;                            // UndCC_Violation(exit) - early return
  }
}

void fn17()                            // UndCC_Violation(decl) - 1 early return
{
  return;                              // UndCC_Violation(exit) - early return
  fn14();
}

void fn18()                            // UndCC_Valid - 1 return at end
{
  return;
  // Comment
}
