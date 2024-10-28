// SEI CERT C++
#ifdef _WIN32
#include <Windows.h>
#include <new>

void *operator new(std::size_t size) noexcept(false) { // UndCC_Violation(Win)
  static HANDLE h = ::HeapCreate(0, 0, 0); // Private, expandable heap.
  if (h) {
    return ::HeapAlloc(h, 0, size);
  }
  throw std::bad_alloc();
}

// No corresponding global delete operator defined.
#endif
