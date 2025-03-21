#include <cstdint>

int32_t fn(int32_t x) // UndCC_Violation
{
    if (x > 0)
    {
        x = x * fn(x - 1);
    }
    return x;
}

constexpr int32_t fn_4(int32_t x) // UndCC_Valid by exception
{
    if (x > 0)
    {
        x = x * fn_4(x - 1);
    }
    return x;
}
constexpr int32_t n = fn_4(6); // Core constant expression
constexpr int32_t fn_5(int32_t x) // UndCC_Violation(1)
{
    if (x > 0)
    {
        x = x * fn_5(x - 1);
    }
    return x;
}
int32_t n1 = fn_5(6);          // Not a core constant expression
template <class T>
auto Sum(T t)
{
    return t;
}
template <class T, class... Vals>
auto Sum(T t, Vals... vals) // UndCC_Valid - calls a different overload
{
    return t + Sum(vals...);
}
