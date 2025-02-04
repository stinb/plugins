#include <stdint.h>

typedef _Bool boolean;
typedef unsigned int UINT_16;

struct s
{
	unsigned int b1_61 : 2;	  /* UndCC_Valid */
	int b2_61 : 2;			  /* UndCC_Violation - plain int not permitted */
	UINT_16 b3_61 : 2;		  /* UndCC_Valid - typedef designating unsigned int */
	signed long b4_61 : 2;	  /* UndCC_Violation even if long and int are the
							   * same size */
	boolean FirstField_b : 1; /* UndCC_Violation(c99) */
};
