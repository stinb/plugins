//% $Id: A3-3-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include "A3-3-1.hpp"
#include <cstdint>

std::int32_t a1 = 0; // UndCC_Valid, external linkage
std::int32_t a2 = 0; // UndCC_Violation, static keyword not used
static std::int32_t a3 = 0; // UndCC_Valid, internal linkage

namespace {
  std::int32_t a4 = 0; // UndCC_Valid by exception
  void F1() // UndCC_Valid by exception
  {

  }
}
namespace n
{
  void F2() // UndCC_Valid, external linkage
  {
  
  }
  std::int32_t a6 = 0; // UndCC_Violation, external linkage
}



static void F3() // UndCC_Valid, static keyword used
{

}

void F4() // UndCC_Valid, external linkage
{
  a1 = 1;
  a2 = 1;
  a3 = 1;
  a4 = 1;
  n::a5 = 1;
  n::a6 = 1;
  //a7 = 1; // Compiler-error
}

void F5() // UndCC_Violation, static keyword not used
{
  a1 = 2;
  a2 = 2;
  a3 = 2;
  a4 = 2;
  n::a5 = 2;
  n::a6 = 2;
  //a7 = 2; // Compiler-error
}

int main(int, char**) // UndCC_Valid by exception
{
  F1();
  n::F2();
  F3();
  F4();
  F5();
}

int F6( int a )
{
  // ..
  return 1;
}
