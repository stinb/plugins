// AUTOSAR

#include <cstdint>
#include <iostream>

void Fn1()
{
  std::int32_t* array =
  new std::int32_t[10]; // new expression used to allocate an
  // array object
  std::cout << array;
  delete array; // UndCC_Violation - array delete expression supposed
  // to be used
}
void Fn2()
{
  std::int32_t* object = new std::int32_t{0}; // new operator used to
  // allocate the memory for an
  // integer type
  // ...
  delete[] object; // UndCC_Violation - non-array delete expression supposed
  // to be used
}
void Fn3()
{
  std::int32_t* object = new std::int32_t{0};
  std::int32_t* array = new std::int32_t[10];
  // ...
  delete[] array; // UndCC_Valid
  delete object; // UndCC_Valid
}

int main()
{
	Fn1();
	return 0;
}
