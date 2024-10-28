#include <cstdint>
#include <functional>

struct A
{
    std::uint8_t x;
};

namespace std {

// UndCC_Valid, case (2)
//template specialization for the user-defined type
//in the same file as the type declaration
template <>
struct hash<A>
{
    size_t operator()(const A& a) const noexcept
    {
        return std::hash<decltype(a.x)>()(a.x);
    }
};

}
