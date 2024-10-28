//% $Id: A7-2-4.cpp 271715 2017-03-23 10:13:51Z piotr.tanski $
#include <cstdint>

enum class Enum1 : std::uint32_t // UndCC_Violation
{
    One,
    Two = 2, 
    Three
};

enum class Enum2 : std::uint32_t // UndCC_Valid (none)
{
    One,
    Two,
    Three
};

enum class Enum3 : std::uint32_t // UndCC_Valid (the first)
{
    One = 1,
    Two,
    Three
};

enum class Enum4 : std::uint32_t // UndCC_Valid (all)
{
    One = 1,
    Two = 2,
    Three = 3
};

enum class Enum5 : std::uint32_t // UndCC_Violation
{
    One = 1,
    Two,
    Three = 3
};
