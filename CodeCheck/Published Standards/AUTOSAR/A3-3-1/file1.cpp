// file1.cpp
#include "header.hpp"

int32_t a1 = 0; // UndCC_Valid
int32_t a2 = 0; // UndCC_Violation
static int32_t a3 = 0; // UndCC_Valid

namespace
{
  int32_t a4 = 0; // UndCC_Valid
  void f1 () // UndCC_Valid
  {
  
  }
}

static void f2 ( ) // UndCC_Valid
{
  //..
}

void f3 ( ) // UndCC_Valid
{
  a1 = 1;
  a2 = 1;
  a3 = 1;
  a4 = 1;
}

void f4 ( ) // UndCC_Violation
{
  a1 = 2;
  a2 = 2;
  a3 = 2;
  a4 = 2;
}

int main ( ) // UndCC_Valid by exception
{
  f1 ( );
  f2 ( );
  f3 ( );
  f4 ( );
}
