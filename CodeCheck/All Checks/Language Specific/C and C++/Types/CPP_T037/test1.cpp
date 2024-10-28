// Custom

#include <cstdint>
#include <numeric>
#include <string>
#include <vector>


void SampleFunction(std::string &a) // UndCC_Valid - Reading before replacing object
{
  a += '\n';
  a = "hello";
}


void NotInOutFunction(std::string b) // UndCC_Valid - not "in-out" parameter
{
  b = b + b;
}


void FunctionReadThenSet(std::string &c) // UndCC_Valid - Reading before replacing object
{
  c += '\n';
  c += '\n';
  c = "hello";
}


void FunctionSetThenRead(std::string &d) // UndCC_Violation - Replacing before reading object
{
  d = "hello";
  d += '\n';
  d += '\n';
}
