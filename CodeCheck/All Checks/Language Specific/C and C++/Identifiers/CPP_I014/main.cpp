#include "posix_typedefs.h"
#include "header.h"

class bar {
	private:
		int WIDTH;
		int HEIGHT;
	public:
		int foobar();
		bar(int w, int h);
};

namespace NS1
{
	typedef int16_t WIDTH;
}

void f1 ( ) {
	typedef int32_t TYPE;
}
void f2 ( ) {
	float32_t TYPE; // UndCC_Violation
}

void f3 ( ) {
	typedef unsigned char uint8_t; /* UndCC_Violation - redefinition */
}

void temp() {
	unsigned char uint8_t; /* UndCC_Violation - reuse of uint8_t */
}

// int uint8_t () { /* Analysis error - reuse of uint8_t */
// 	return 4311;
// }

int main() {
	int WIDTH; /* UndCC_Violation - reuse of WIDTH */
	WIDTH = 1;
	return WIDTH;
}
