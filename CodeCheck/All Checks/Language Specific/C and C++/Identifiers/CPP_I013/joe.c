#include "posix_typedefs.h"
#include "joe.h"

struct stag { uint16_t a; uint16_t b; }; /* Not Compliant, redefinition */
struct stag a1 = { 0, 0 }; /* UndCC_Valid - compatible with above */
union stag a2 = { 0, 0 }; /* Not compliant - not compatible with previous declarations */ // UndCC_Violation
void foo(void)
{
	struct stag { uint16_t a; }; /* Not compliant - tag stag redefined */
}

int main() {
	return 1;
}
