// MISRA C++ 2023 Rule 8.2.4: Casts shall not be performed between a pointer to
// function and any other type
//
// Test cases for function pointer casting

#include <cstdint>
#include <cstddef>

using pf16_t = void (*)(int16_t n);
using pf32_t = void (*)(int32_t n);
using pf_noexcept = void (*)(int16_t n) noexcept;

pf16_t getPf16();

// Test nullptr to function pointer - COMPLIANT
void test_nullptr_to_function_pointer()
{
  pf16_t p1 = static_cast<pf16_t>(nullptr); // UndCC_Valid
  pf16_t p2 = nullptr; // UndCC_Valid
  pf16_t p3 = NULL; // UndCC_Valid
  pf16_t p4 = 0; // UndCC_Valid
}

// Test cast to void - COMPLIANT (exception)
void test_cast_to_void()
{
  (void)getPf16(); // UndCC_Valid
}

// Test function pointer to different function pointer - NON-COMPLIANT
void test_incompatible_function_pointers()
{
  pf16_t p1 = static_cast<pf16_t>(nullptr);
  pf32_t p2 = reinterpret_cast<pf32_t>(p1); // UndCC_Violation
}

// Test integer to function pointer - NON-COMPLIANT
void test_integer_to_function_pointer()
{
  pf16_t p3 = (pf16_t)0x8000; // UndCC_Violation
  pf16_t p4 = reinterpret_cast<pf16_t>(0xdeadbeef); // UndCC_Violation
}

// Test function pointer to object pointer - NON-COMPLIANT
void test_function_pointer_to_object_pointer()
{
  pf16_t p4 = reinterpret_cast<pf16_t>(0xdeadbeef); // UndCC_Violation
  int16_t *p5 = reinterpret_cast<int16_t *>(p4); // UndCC_Violation
}

// Test noexcept function pointer conversion - COMPLIANT (noexcept -> normal)
void test_noexcept_conversion()
{
  pf_noexcept pne = nullptr;
  pf16_t p6 = static_cast<pf16_t>(pne); // UndCC_Valid - allowed direction
}

// Test reverse noexcept with reinterpret_cast - NON-COMPLIANT
void test_noexcept_reinterpret()
{
  pf16_t pn = nullptr;
  pf_noexcept p7 = reinterpret_cast<pf_noexcept>(pn); // UndCC_Violation - wrong direction
}

// Test overload resolution - COMPLIANT
void f5();
void f5(int16_t);

template<typename T>
void f6(T);

void test_overload_resolution()
{
  f6(static_cast<void (*)()>(f5)); // UndCC_Valid
}

// Test pointer to member function - NON-COMPLIANT
struct A
{
  void foo();
  int32_t i;
};

struct B : A
{
};

void test_pointer_to_member_function()
{
  auto pm1 = static_cast<void (B::*)()>(&A::foo); // UndCC_Violation
}

// Test pointer to data member - NON-COMPLIANT
void test_pointer_to_data_member()
{
  auto pm2 = static_cast<int32_t (B::*)>(&A::i); // UndCC_Violation
}

// Test contextual conversion to bool - COMPLIANT (no cast)
void test_contextual_conversion()
{
  pf16_t p1 = nullptr;
  if (p1) // UndCC_Valid
  {
    // Contextually converted to bool - no cast involved
  }
}

// Test function-to-pointer conversion - COMPLIANT
void someFunc(int16_t x);

void test_function_to_pointer()
{
  pf16_t p7 = static_cast<pf16_t>(someFunc); // UndCC_Valid
  pf16_t p8 = someFunc; // UndCC_Valid
}

// Test same type cast (identity cast)
void test_same_type_cast()
{
  pf16_t p1 = nullptr;
  pf16_t p2 = static_cast<pf16_t>(p1); // UndCC_Valid
}

// Test implicit conversions - these should be OK (no explicit cast)
void test_implicit_conversions()
{
  void (*f1)(int16_t) = nullptr; // UndCC_Valid - nullptr to function pointer (implicit)

  void someFunc2(int16_t);
  pf16_t f2 = someFunc2; // UndCC_Valid - function to pointer (implicit decay)

  pf16_t f3 = nullptr;
  bool b = f3; // UndCC_Valid - contextual conversion to bool (implicit)
}

// Test function pointer to void* - NON-COMPLIANT
void test_function_pointer_to_void_ptr()
{
  pf16_t p1 = nullptr;
  void* vp = reinterpret_cast<void*>(p1); // UndCC_Violation
  pf16_t p2 = reinterpret_cast<pf16_t>(vp); // UndCC_Violation
}

// Test C-style casts - NON-COMPLIANT
void test_c_style_cast()
{
  pf16_t p1 = nullptr;
  int* ip = (int*)p1; // UndCC_Violation
  pf16_t p2 = (pf16_t)ip; // UndCC_Violation
}

// Test lambda to function pointer - COMPLIANT
void test_lambda_to_function_pointer()
{
  auto lambda = []() {};
  void (*fp)() = lambda; // UndCC_Valid - implicit conversion (no explicit cast)
  void (*fp2)() = static_cast<void(*)()>(lambda); // UndCC_Valid - standard conversion
}
