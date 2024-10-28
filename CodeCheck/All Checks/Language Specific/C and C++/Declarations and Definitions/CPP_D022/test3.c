#include "test0.h"
#include "test1.h"
#include <stdio.h>

int global_variable2 = 37; // UndCC_Violation

void use_it( void ) {
	printf( "Global variable: %d\n", global_variable++ );
}

