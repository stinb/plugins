#include <cstdint>

void f ( )
{
	float f32;
	int32_t s32;
	s32 = f32;                          // UndCC_Violation
	f32 = s32;                          // UndCC_Violation
	f32 = static_cast< float > ( s32 ); // UndCC_Valid
}
