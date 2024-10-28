struct SomeStruct {
	unsigned int validBoolean : 1;
	signed int invalidField : 1; // UndCC_Violation
	signed int validField1 : 2;
	signed int validField2 : 4;
	unsigned int validField3 : 2;
	signed int validZeroLength1 : 0;
	unsigned int validZeroLength2 : 0;
	signed int : 1; // valid anonymous
	unsigned int : 1; // valid anonymous
};
