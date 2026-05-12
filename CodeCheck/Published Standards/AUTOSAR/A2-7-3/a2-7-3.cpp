//% $Id: A2-7-3.hpp 305382 2018-01-26 06:32:15Z michal.szczepankiewicz $

typedef short new_int; // UndCC_Violation

/// union sample
/// @brief union for S
union S
{
    int n;                  // occupies 4 bytes // UndCC_Violation
    unsigned short s[2];    // occupies 4 bytes // UndCC_Violation
    unsigned char c;        // occupies 1 byte // UndCC_Violation
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

void F1(int) noexcept; // UndCC_Violation documentation

int F2(short input1, // UndCC_Violation
       int input2);

/// @brief Function description
///
/// @param input1 input1 parameter description
/// @param input2 input2 parameter description
/// @throw std::runtime_error conditions to runtime_error occur
///
/// @return return value description
int F3(
    short input1,
    short input2) noexcept(false); // UndCC_Valid documentation

/// @brief Class responsibility
class C // UndCC_Valid documentation
{
public:
    /// @brief Constructor description
    ///
    /// @param input1 input1 parameter description
    /// @param input2 input2 parameter description
    C(int input1, float input2) : x{input1}, y{input2} {}

    /// @brief Method description
    ///
    /// @return return value descrption
    int const *GetX() const noexcept { return &x; }

private:
    /// @brief Data member descpription
    int x;
    /// @brief Data member descpription
    float y;
};
