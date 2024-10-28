//% $Id: A2-7-3.hpp 305382 2018-01-26 06:32:15Z michal.szczepankiewicz $
#include <cstdint>

typedef int16_t new_int; // UndCC_Violation

/// union sample
/// @brief union for S
union S
{
    std::int32_t n;     // occupies 4 bytes // UndCC_Violation
    std::uint16_t s[2]; // occupies 4 bytes // UndCC_Violation
    std::uint8_t c;     // occupies 1 byte // UndCC_Violation
};

struct product // UndCC_Violation
{
    int weight; // UndCC_Violation
    double price; // UndCC_Violation
};

enum colors // UndCC_Violation
{
    red = 5,
    black
};

void F1(std::int32_t) noexcept; // UndCC_Violation documentation

std::int32_t F2(std::int16_t input1, // UndCC_Violation
                std::int32_t input2); 

/// @brief Function description
///
/// @param input1 input1 parameter description
/// @param input2 input2 parameter description
/// @throw std::runtime_error conditions to runtime_error occur
///
/// @return return value description
std::int32_t F3(
    std::int16_t input1,
    std::int16_t input2) noexcept(false); // UndCC_Valid documentation

/// @brief Class responsibility
class C // UndCC_Valid documentation
{
public:
    /// @brief Constructor description
    ///
    /// @param input1 input1 parameter description
    /// @param input2 input2 parameter description
    C(std::int32_t input1, float input2) : x{input1}, y{input2} {}

    /// @brief Method description
    ///
    /// @return return value descrption
    std::int32_t const *GetX() const noexcept { return &x; }

private:
    /// @brief Data member descpription
    std::int32_t x;
    /// @brief Data member descpription
    float y;
};
