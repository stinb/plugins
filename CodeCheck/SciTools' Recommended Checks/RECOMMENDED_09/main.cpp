#include "header.h"

int fun () {
	/* called in main */
	return 42;
}
int notfun () {
	/* not called */
	return 0;
}

int main() {
	/* bad /* comment */ // UndCC_Violation
	return fun();
}