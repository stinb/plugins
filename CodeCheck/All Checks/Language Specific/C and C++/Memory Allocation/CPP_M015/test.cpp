#include <iostream>
class A {
  static void* operator new(std::size_t sz)
  {
    std::cout << "custom new for size " << sz << '\n';
    return ::operator new(sz);
  }
  
  static void* operator new[](std::size_t sz) // UndCC_Violation
  {
    std::cout << "custom new[] for size " << sz << '\n';
    return ::operator new(sz);
  }
};

class A1 {
  static void* operator new(std::size_t sz)
  {
    std::cout << "custom new for size " << sz << '\n';
    return ::operator new(sz);
  }
  // UndCC_Valid
  static void* operator new[](std::size_t sz)
  {
    std::cout << "custom new[] for size " << sz << '\n';
    return ::operator new(sz);
  }
  static void operator delete(void* ptr, std::size_t sz)
  {
    std::cout << "custom delete for size " << sz << '\n';
    ::operator delete(ptr);
  }
  static void operator delete[](void* ptr, std::size_t sz)
  {
    std::cout << "custom delete for size " << sz << '\n';
    ::operator delete(ptr);
  }
};
