#include <cstdint>

typedef int32_t myInt;
typedef void myVoid;
typedef myInt obscureInt;

enum class Color { Red, Green, Blue };

struct S
{
  int32_t i;
  int32_t j;
};

void f(void* v, int32_t i)
{
  char* v2;
  S* s1 = reinterpret_cast<S*>(v); // UndCC_Violation(0,1,2)
  S* s2 = reinterpret_cast<S*>(i); // UndCC_Violation(1,2)
  S* s3;

  s3 = reinterpret_cast<S*>(v2); // UndCC_Valid
}

void f2(void* p1, int32_t i)
{
  S* s1 = static_cast<S*>(p1); // UndCC_Violation(0,1,2)
  S* s2 = reinterpret_cast<S*>(i); // UndCC_Violation(1,2)
  void* p2 = reinterpret_cast<void*>(i); // UndCC_Violation(1,2)
  auto p3 = const_cast<void const*>(p2); // UndCC_Valid
}

void g(myVoid* v, myInt i)
{
  char* v2;
  S* s2 = reinterpret_cast<S*>(i); // UndCC_Violation(1,2)
  S* s1 = reinterpret_cast<S*>(v); // UndCC_Violation(0,1,2)
}

void h(void* v, obscureInt i)
{
  S* s1 = reinterpret_cast<S*>(v); // UndCC_Violation(0,1,2)
  S* s2 = reinterpret_cast<S*>(i); // UndCC_Violation(1,2)
}

void enumToPointerExample(void* v)
{
  Color color = Color::Red;
  void* ptr = reinterpret_cast<void*>(color); // UndCC_Violation(2)
  S* sPtr = reinterpret_cast<S*>(color); // UndCC_Violation(2)

}

