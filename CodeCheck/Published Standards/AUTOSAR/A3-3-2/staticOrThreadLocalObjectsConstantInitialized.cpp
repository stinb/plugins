// $Id: A3-3-2.cpp 305690 2018-01-29 14:35:00Z jan.babst $
#include <cstdint>
#include <limits>
#include <string>
class A
{
    public:
        static std::uint8_t instanceId;
        static float const pi;
        static std::string const separator;

        A() {}
        // Implementation...
};
std::uint8_t A::instanceId = 0; // UndCC_Valid - constant initialization
float const A::pi = 3.14159265359; // UndCC_Valid - constant initialization
std::string const A::separator = // UndCC_Violation
"=========="; 

class C
{
    public:
        constexpr C() = default;
};

namespace
{
    constexpr std::int32_t maxInt32 =
        std::numeric_limits<std::int32_t>::max(); // UndCC_Valid - constexpr variable

    A instance{}; // UndCC_Valid - constant (value) initialization
    constexpr C c{}; // UndCC_Valid - constexpr c’tor call
} // namespace

void Fn() noexcept
{
    static std::int32_t counter{0}; // UndCC_Valid
    static std::string border(5, '*'); // UndCC_Violation - not a constexpr c’tor
}

class D
{
    public:
        D() = default;
        D(D const&) = default;
        D(D&&) = default;
        D& operator=(D const&) = default;
        D& operator=(D&&) = default;
        ~D() = default;

    private:
        static D* instance;
};
D* D::instance = nullptr; // UndCC_Valid - initialization by constant expression
