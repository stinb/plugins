#include <cstddef>
#include <cstdint>

struct A
{
   int32_t i;
};
void f1()
{
   offsetof(A, i); // UndCC_Violation
}