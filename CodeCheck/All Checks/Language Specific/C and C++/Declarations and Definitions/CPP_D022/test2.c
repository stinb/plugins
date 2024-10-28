#include "test0.h"
#include "test1.h"

int global_variable = 37;

int increment( void ) {
	return global_variable++;
}
