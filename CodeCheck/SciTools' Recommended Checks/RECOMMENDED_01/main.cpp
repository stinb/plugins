#include "header.h"

void class3::okay1() {}
int fun () {
	/* called in other file */
	return 42;
}
int notfun () {
	/* not called */
	return 0;
}

int main() {
	/* main ignored */
	return 0;
}
