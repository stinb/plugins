// $Id: A9-6-1.cpp 319312 2018-05-15 08:29:17Z christof.meerwald $
#include <cstdint>

enum class E1 : std::uint8_t
{
	E11,
	E12,
	E13
};
enum class E2 : std::int16_t
{
	E21,
	E22,
	E23
};
enum class E3 // UndCC_Violation
{
	E31,
	E32,
	E33
};
enum E4 // UndCC_Violation
{
	E41,
	E42,
	E43
};

class C
{
public:
	std::int32_t a : 2; // UndCC_Valid
	std::uint8_t b : 2U; // UndCC_Valid

	bool c : 1; // UndCC_Violation - the size of bool is implementation defined

	char d : 2; // UndCC_Violation
	wchar_t e : 2; // UndCC_Violation - the size of wchar_t is implementation defined

	E1 f1 : 2; // UndCC_Valid
	E2 f2 : 2; // UndCC_Valid
	E3 f3 : 2; 
	// underlying type
	E4 f4 : 2; 
	// type
};

struct D
{
	std::int8_t a; // UndCC_Valid

	bool b; // UndCC_Violation - the size of bool is
	// implementation defined

	std::uint16_t c1 : 8; // UndCC_Valid
	std::uint16_t c2 : 8; // UndCC_Valid
};

	void Fn() noexcept
{
	C c;
	c.f1 = E1::E11;
}
