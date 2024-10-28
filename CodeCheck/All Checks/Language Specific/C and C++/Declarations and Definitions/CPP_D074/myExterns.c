#include <stdio.h>

extern int var1 = 0; // flags violation // UndCC_Violation

extern int* var4 = 2; // flags violation // UndCC_Violation

extern int** var5 = 5; // flags violation // UndCC_Violation

extern int var2;

var2 = 3;

extern int var3;

int main() {
	
	var2 = 7;

    return 0;
}
