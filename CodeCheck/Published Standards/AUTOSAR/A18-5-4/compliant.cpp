//% $Id: A18-5-4.cpp 289415 2017-10-04 09:10:20Z piotr.serwa $
#include <cstdlib>
void operator delete(
  void* ptr) noexcept // UndCC_Valid - sized version is defined
{
  std::free(ptr);
}
void operator delete(
  void* ptr,
std::size_t size) noexcept // UndCC_Valid - unsized version is defined
{
  std::free(ptr);
}
