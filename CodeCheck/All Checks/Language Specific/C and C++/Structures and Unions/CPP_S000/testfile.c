enum Suit {
	Diamonds,
	Hearts,
	Clubs,
	Spades
};

union DATATYPE { /* UndCC_Violation */
	char   ch;
	int    i;
	long   l;
	float  f;
	double d;
};

struct LameStruct {
	union DATATYPE invalidMember; /* UndCC_Violation */
	int validMember;
	enum Suit validEnumMember;
};

typedef union DATATYPE invalid_t; /* UndCC_Violation */

struct LameStruct validObject;
enum Suit validEnumObject;
union DATATYPE invalidObject1; /* UndCC_Violation */
invalid_t invalidObject2; /* UndCC_Violation */

typedef union test { // UndCC_Violation
	int i;
	double j;
} test; // UndCC_Violation
