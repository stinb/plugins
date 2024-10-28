// AUTOSAR

#include <stdexcept>
#include <type_traits>

class C1
{
public:
  C1() = default;

  // UndCC_Valid - move constructor is non-throwing and declared to be noexcept
  C1(C1&& rhs) noexcept {}

  // UndCC_Valid - move assignment operator is non-throwing and declared to be
  // noexcept
  C1& operator=(C1&& rhs) noexcept { return *this; }

  // UndCC_Valid - destructor is non-throwing and declared to be noexcept by
  // default
  ~C1() noexcept {}
};

void Swap(C1& lhs,
          C1& rhs) noexcept // UndCC_Valid - swap function is non-throwing and
                            // declared to be noexcept
{
  // Implementation
}

class C2
{
public:
  C2() = default;
  // UndCC_Valid - move constructor is non-throwing and declared to be noexcept
  C2(C2&& rhs) noexcept
  {
    try
    {
      // ...
      throw std::runtime_error(
      "Error"); // Exception will not escape this function
    }
    catch (std::exception& e)
    {
      // Handle error
    }
  }
  C2& operator=(C2&& rhs) noexcept
  {
    try
    {
      // ...
      throw std::runtime_error(
      "Error"); // Exception will not escape this function
    }
    catch (std::exception& e)
    {
      // Handle error
    }
    return *this;
  }
  // UndCC_Valid - destructor is non-throwing and declared to be noexcept by
  // default
  ~C2() // UndCC_Violation
  {
    try
    {
      // ...
      throw std::runtime_error(
      "Error"); // Exception will not escape this function
    }
    catch (std::exception& e)
    {
      // Handle error
    }
  }
};

void Swap(C2& lhs, C2& rhs) noexcept(false)
{
  // ...

  throw std::runtime_error("Swap function failed");
}

class C3
{
public:
  C3() = default;
  C3(C3&& rhs) // UndCC_Violation - move constructor throws
  {
    // ...
    throw std::runtime_error("Error");
  }
  C3& operator=(C3&& rhs) // UndCC_Violation - move assignment operator throws
  {
    // ...
    throw std::runtime_error("Error");
    return *this;
  }
  ~C3() // UndCC_Violation - destructor exits with an exception
  {
    throw std::runtime_error("Error");
  }
  static void operator delete(void* ptr, std::size_t sz)
  {
    // ...
    throw std::runtime_error("Error");
    // function exits with an exception
  }
};

void Fn()
{
  C3 c1; // program terminates when c1 is destroyed
  C3* c2 = new C3;
  // ...
  delete c2; // program terminates when c2 is deleted
}

template <typename T>
class Optional
{
public:
  // ...
  // UndCC_Valid by exception
  Optional(Optional&& other) noexcept(
  std::is_nothrow_move_constructible<T>::value)
  {
    // ...
  }
  // UndCC_Valid by exception
  Optional& operator=(Optional&& other) noexcept(
  std::is_nothrow_move_assignable<T>::value&&
  std::is_nothrow_move_constructible<T>::value)
  {
    // ...
    return *this;
  }
  // ...
};
