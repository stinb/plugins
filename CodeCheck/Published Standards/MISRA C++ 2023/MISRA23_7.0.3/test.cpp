#include <cstdint>
#include <string>
#include <optional>
#include <locale>
#include <istream>

char a = 'a';       // Rule does not apply - no conversion
char b = '\r';      // Rule does not apply - no conversion
char c = 10;        // UndCC_Violation - implicit conversion from int to char
int8_t d = 'a';     // UndCC_Violation
uint8_t e = '\r';   // UndCC_Violation
signed char f = 11; // Rule does not apply - type has integral category
using CT = std::char_traits<char>;
char g = L'Æ';    // UndCC_Violation - conversion between character types
char h = a - '0'; // UndCC_Violation - promotion to int, conversion to char

void foo()
{
    if (g && h) // UndCC_Violation - two conversions to bool
    {
    }
    if (a != 'q') // Rule does not apply - comparing the same types
    {
    }
    if (CT::eq(a, 'q')) // Rule does not apply - no conversion
    {
    }

    std::optional<char> o;

    if (o == 'r') // Rule does not apply - no conversion
    {
    }

    decltype('s' + 't') w; // Rule does not apply - unevaluated operand

    auto i = static_cast<CT::int_type>('a'); // UndCC_Violation - explicitly
                                             // converted to CT::int_type
    auto j = CT::to_int_type('a');           // Rule does not apply
                                             // - no conversion
    if (('0' >= a) && (a <= '9'))            // UndCC_Violation - promotion to int
    {
    }

    if (!CT::lt(a, '0') && !CT::lt('9', a)) // UndCC_Valid version of the above
    {
    }

    if (0 == std::isdigit(a)) // UndCC_Violation - conversion to int
    {
    }

    if (std::isdigit(a, std::locale{})) // UndCC_Valid version of the above
    {
    }
}

void f1(std::istream &is)
{
    auto i = is.get();

    if (CT::not_eof(i))
    {
        char c1 = i;                   // UndCC_Violation - int to char
        char c2 = CT::to_char_type(i); // UndCC_Valid version of the above
    }
}
