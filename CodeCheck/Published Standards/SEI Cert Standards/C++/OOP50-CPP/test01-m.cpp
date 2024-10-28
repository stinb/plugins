
#include "test01-m.h"

ClassA::ClassA()
{
  fun01(); // UndCC_Violation
}

ClassA::~ClassA()
{
  fun02(); // UndCC_Valid
}
