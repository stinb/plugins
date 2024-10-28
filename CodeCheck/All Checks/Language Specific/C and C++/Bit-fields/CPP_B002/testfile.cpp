#include "posix_typedefs.h"

typedef bool boolean;
typedef boolean boolean_indirect;

typedef uint32_t uint32_indirect;

enum AnEnumType {
	A,
	B,
	C,
};

typedef AnEnumType AnIndirectEnumType;

struct SomeStruct {
	int validField : 4;
	signed int validSigned : 2;
	unsigned int validUnsigned: 1;
	char validChar : 2;
	unsigned char validUnsignedChar : 2;
	signed char validSignedChar : 2;
	bool validBool : 1;
	boolean validBoolean : 1;
	boolean_indirect validIndirectBoolean : 1;
	uint32_t validUint32 : 8;
	char_t validCharT : 3;
	uint32_indirect validIndirectUint32 : 7;
	
	AnEnumType invalidEnum : 2; // UndCC_Violation
	AnIndirectEnumType invalidIndirectEnum : 2; // UndCC_Violation
};
