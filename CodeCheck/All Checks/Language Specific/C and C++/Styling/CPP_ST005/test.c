#include <stdio.h>

/* bad example */
enum
{
	a,
	b,
	c=0, // UndCC_Violation
};

/* good example */
enum
{
	d,
	e,
	f=0
};
