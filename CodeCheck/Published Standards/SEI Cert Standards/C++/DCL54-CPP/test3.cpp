// SEI CERT C++

#include <new>

extern "C++" void update_bookkeeping(void *allocated_ptr, std::size_t size, bool alloc);

struct S {
  void *operator new(std::size_t size) noexcept(false) { // UndCC_Valid
    void *ptr = ::operator new(size);
    update_bookkeeping(ptr, size, true);
    return ptr;
  }

  void operator delete(void *ptr, std::size_t size) noexcept { // UndCC_Valid
    ::operator delete(ptr);
    update_bookkeeping(ptr, size, false);
  }
};
