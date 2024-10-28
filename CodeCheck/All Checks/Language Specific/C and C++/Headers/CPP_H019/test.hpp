// UndCC_Valid
class TargetClass;
class SourceClass
{
  TargetClass *pTargetClass;
};


#include "TargetClass.hpp" // UndCC_Violation
class SourceClass1 {
  TargetClass *pTargetClass;
};
