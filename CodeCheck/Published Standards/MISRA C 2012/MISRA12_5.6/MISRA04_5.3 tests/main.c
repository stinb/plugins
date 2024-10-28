#include "header.h"


typedef unsigned char uint8_t; /* UndCC_Violation - redefinition */


void temp() {
	unsigned char uint8_t; /* UndCC_Violation - reuse of uint8_t */
}

#ifdef __UNDERSTAND_FUZZY_PARSE__
int uint8_t (){ /* UndCC_Violation - reuse of uint8_t */
	return 4311;
}
#endif

int main() {
	return 1;
}
