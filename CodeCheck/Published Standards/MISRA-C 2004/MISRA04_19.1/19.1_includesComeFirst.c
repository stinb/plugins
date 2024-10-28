#include <stdint.h>
#include <f1.h> // UndCC_Valid
//test this comment
	//test comment again
#define HELLO
#include <f2.h> // UndCC_Valid
	int32_t i;
	void func1();
void func2();
#include <f3.h> // UndCC_Violation
#include <stdio.h>  // UndCC_Violation
    #include <stdlib.h>  // UndCC_Violation

#if HELLO


