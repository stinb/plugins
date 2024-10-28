#ifndef OKAY1
#define OKAY1
#endif

#ifndef MACRO2
#endif

#define MACROVIOLATION

class class1{
	#define VIOLATION1 // UndCC_Violation
};

void function1(){
#ifdef OKAY1
  int i=0;
#endif
	#define VIOLATION2 // UndCC_Violation
	#undef MACROVIOLATION // UndCC_Violation
}

namespace name1{
	#define VIOLATION3 // UndCC_Violation
	#undef 	VIOLATION4 // UndCC_Violation
}
