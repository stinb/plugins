//% $Id: A18-5-4.cpp 289415 2017-10-04 09:10:20Z piotr.serwa $
#include <cstdlib>
void operator delete( // UndCC_Violation
  void* ptr) noexcept 
{
  std::free(ptr);
}
