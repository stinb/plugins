struct SomeStruct {
	int invalidField : 4; // UndCC_Violation
	signed int validSigned : 2; // UndCC_Violation
	unsigned int validUnsigned: 1;
	char invalidChar : 2; // UndCC_Violation
	unsigned char invalidUnsignedChar : 2; // UndCC_Violation
	signed char invalidSignedChar : 2; // UndCC_Violation
};


typedef unsigned int bitfield_t;

typedef struct {
    bitfield_t     valid1 : 1;
	bitfield_t     valid2 : 1;
    unsigned int   valid : 1;
} test_t;

test_t  test_var;
