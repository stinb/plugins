#include <cstdint>

// Integral to Float
void f1 ( )
{
	int16_t s16a;
	int16_t s16b;
	int16_t s16c;
	float f32a;
	// The following performs integer division
	f32a = static_cast< float > ( s16a / s16b ); // UndCC_Violation
	// The following also performs integer division
	s16c = s16a / s16b;
	f32a = static_cast< float > ( s16c ); // UndCC_Valid
	// The following performs floating-point division
	f32a = static_cast< float > ( s16a ) / s16b; // UndCC_Valid
}

// Float to Integral
void f2 ( )
{
	float f32a;
	float f32b;
	float f32c;
	int16_t s16a;
	// The following performs floating-point division
	s16a = static_cast< int16_t > ( f32a / f32b ); // UndCC_Violation
	// The following also performs floating-point division
	f32c = f32a / f32b;
	s16a = static_cast< int16_t > ( f32c ); // UndCC_Valid
	// The following performs integer division
	s16a = static_cast< int16_t > ( f32a ) / f32b; // UndCC_Valid
}
