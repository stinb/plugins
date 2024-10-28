#include <cstdint>

struct S
{
    signed int a : 2;  // UndCC_Valid
    int : 2;           // UndCC_Valid
    int32_t b : 2;     // UndCC_Valid
    char c : 2;        // UndCC_Violation
    signed char d : 2; // UndCC_Valid - signed integer type
    wchar_t e : 2;     // UndCC_Violation - not a signed or unsigned integer type
    char32_t f : 2;    // UndCC_Violation - not a signed or unsigned integer type
    bool g : 1;        // UndCC_Valid
};
enum Direction : int32_t
{
    Top,
    Left,
    Bottom,
    Right
};
enum Colour : char
{
    Red,
    Pink,
    Blue
};
enum Line : unsigned char
{
    Plain,
    Dash,
    Dot
};
struct S2
{
    Direction dir : 4;     // UndCC_Violation - unscoped and no underlying type
    Colour lineColour : 2; // UndCC_Violation - underlying type is plain char
    Line lineStyle1 : 1;   // UndCC_Violation - cannot represent Dot
    Line lineStyle2 : 2;   // UndCC_Valid
};
