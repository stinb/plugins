// $Id: A7-1-9.cpp 305629 2018-01-29 13:29:25Z piotr.serwa $
#include <cstdint>

enum class DIRECTION
{
    UP,
    DOWN
} dir;  // UndCC_Violation

class Foo
{
public:
    enum class ONE {AA, BB}; // UndCC_Valid

    static constexpr enum class TWO {CC, DD} sVar = TWO::CC; // UndCC_Violation
    static constexpr ONE sVar2 = ONE::AA; // UndCC_Valid
};


struct Bar
{
    std::uint32_t a;
} barObj;  // UndCC_Violation

struct Bar2
{
    std::uint32_t a;
} bar2Obj, *bar2Ptr;  // UndCC_Violation

struct Foo2
{
    std::uint32_t f;
};

Foo2 foo2Obj; // UndCC_Valid
