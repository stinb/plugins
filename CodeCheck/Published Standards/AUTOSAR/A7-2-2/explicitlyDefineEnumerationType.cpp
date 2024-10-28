// $Id: A7-2-2.cpp 271715 2017-03-23 10:13:51Z piotr.tanski $
// g++ -Wconversion-null -std=c++14 main.cpp -o main.exe
#include <cstdint>

enum class E1 // UndCC_Violation(0)
{
	E10,
	E11,
	E12
};

enum class E2 : std::uint8_t // UndCC_Valid
{
	E20,
	E21,
	E22
};

enum E3 // UndCC_Violation(0)
{
	E30,
	E31,
	E32
};

enum E4 : std::uint8_t // UndCC_Valid - violating another rule
{
	E40,
	E41,
	E42
};

enum class E5 : std::uint8_t
{
	E50 = 255,
	// E5_1, // E5_1 = 256 which is outside of range of underlying type std::uint8_t
	// - compilation error
	// E5_2 // E5_2 = 257 which is outside of range of underlying type std::uint8_t
	// - compilation error
};

enum E6 : int // UndCC_Valid
{
	E60,
	E61
};

int main()
{
	return 0;
}
