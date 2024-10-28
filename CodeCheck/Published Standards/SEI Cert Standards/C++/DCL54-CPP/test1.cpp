// SEI CERT C++
#ifdef _WIN32
#include <Windows.h>
#include <new>

class HeapAllocator {
  static HANDLE h;
  static bool init;

public:
  static void *alloc(std::size_t size) noexcept(false) {
    if (!init) {
      h = ::HeapCreate(0, 0, 0); // Private, expandable heap.
      init = true;
    }

    if (h) {
      return ::HeapAlloc(h, 0, size);
    }
    throw std::bad_alloc();
  }

  static void dealloc(void *ptr) noexcept {
    if (h) {
      (void)::HeapFree(h, 0, ptr);
    }
  }
};

HANDLE HeapAllocator::h = nullptr;
bool HeapAllocator::init = false;

void *operator new(std::size_t size) noexcept(false) { // UndCC_Valid
  return HeapAllocator::alloc(size);
}

void operator delete(void *ptr) noexcept { // UndCC_Valid
  return HeapAllocator::dealloc(ptr);
}
#endif
