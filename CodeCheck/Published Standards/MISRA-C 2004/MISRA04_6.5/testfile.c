struct SomeStruct {
	unsigned int validBoolean : 1;
	signed int invalidField : 1; // UndCC_Violation
	signed int validField1 : 2;
	signed int validField2 : 4;
	unsigned int validField3 : 2;
	signed int invalidZeroLength : 0; // UndCC_Violation
	unsigned int validZeroLength : 0;
};
