// $Id: A18-5-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
#include <cstdlib>
void F1() noexcept(false)
{
  
  std::int32_t* p1 = static_cast<std::int32_t*>(malloc(sizeof(std::int32_t))); // UndCC_Violation
  *p1 = 0;

  // UndCC_Valid
  std::int32_t* p2 = new std::int32_t(0);

  // UndCC_Valid
  delete p2;

  
  free(p1); // UndCC_Violation

  
  std::int32_t* array1 =
  static_cast<std::int32_t*>(calloc(10, sizeof(std::int32_t))); // UndCC_Violation

  
  std::int32_t* array2 =
  static_cast<std::int32_t*>(realloc(array1, 10 * sizeof(std::int32_t))); // UndCC_Violation

  // UndCC_Valid
  std::int32_t* array3 = new std::int32_t[10];

  // UndCC_Valid
  delete[] array3;

  
  free(array2); // UndCC_Violation

  
  free(array1); // UndCC_Violation
}

void F2() noexcept(false)
{
  
  std::int32_t* p1 = static_cast<std::int32_t*>(malloc(sizeof(std::int32_t))); // UndCC_Violation
  
  delete p1;

  std::int32_t* p2 = new std::int32_t(0); // UndCC_Valid
  free(p2); // UndCC_Violation - undefined behavior
}

void operator delete(void* ptr) noexcept
{
  std::free(ptr); // UndCC_Valid by exception
}
