/*#include "joe.h" */
#include <stdio.h>

int main() {
	const char * st = "enter date as ??-??-??"; /* bad use of ?'s (can make trigraphs) */ // UndCC_Violation
	printf("%s\n", st); /* prints ~~?? */
	int a = 0;
	// Will the next line be executed????????????????/
	a++;
	// ... depends on your compiler!
	return a;
}
