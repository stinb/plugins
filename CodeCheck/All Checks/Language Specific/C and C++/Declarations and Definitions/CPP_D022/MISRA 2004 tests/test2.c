#include "test1.h"
#include "test2.h"

int sharedNumber = 20;

int privateFunction() { // UndCC_Violation
	return getShared();
}
