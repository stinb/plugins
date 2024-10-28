#include "other.h"
#include "header.h"

int valid2() {
	/* remotely called function */
	return 100 + othervalid();
}

int othervalid() {
	/* locally called function */
	return valid2() * 10;
}
