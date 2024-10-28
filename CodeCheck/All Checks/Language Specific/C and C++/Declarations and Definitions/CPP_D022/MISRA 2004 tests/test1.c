#include "test1.h"
#include "test2.h"

int privateNumber = 20; // UndCC_Violation

int getShared() {
	return sharedNumber;
}
