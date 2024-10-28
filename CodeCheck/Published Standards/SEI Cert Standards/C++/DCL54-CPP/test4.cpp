// Custom - new, delete, new[], and delete[]

#include <new>
#include <cstdlib>

struct Both {
  void *operator new(std::size_t count)
  {
    count = count ?: 1;

    if (void *ptr = std::malloc(count))
      return ptr;

    throw std::bad_alloc{};
  }

  void operator delete(void *ptr)
  {
    std::free(ptr);
  }
};

struct New {
  void *operator new(std::size_t count) // UndCC_Violation
  {
    count = count ?: 1;

    if (void *ptr = std::malloc(count))
      return ptr;

    throw std::bad_alloc{};
  }
};

struct Delete {
  void operator delete(void *ptr) // UndCC_Violation
  {
    std::free(ptr);
  }
};

struct BothArray {
  void *operator new[](std::size_t count)
  {
    count = count ?: 1;

    if (void *ptr = std::malloc(count))
      return ptr;

    throw std::bad_alloc{};
  }

  void operator delete[](void *ptr)
  {
    std::free(ptr);
  }
};

struct NewArray {
  void *operator new[](std::size_t count) // UndCC_Violation
  {
    count = count ?: 1;

    if (void *ptr = std::malloc(count))
      return ptr;

    throw std::bad_alloc{};
  }
};

struct DeleteArray {
  void operator delete[](void *ptr) // UndCC_Violation
  {
    std::free(ptr);
  }
};
