typedef unsigned short UINT_16;

struct s {
	
	unsigned int b1_61:2; 	/* UndCC_Valid */
	int b2_61:2; 			/* UndCC_Violation - plain int not permitted */
	UINT_16 b3_61:2; 		/* UndCC_FalsePos - typedef designating unsigned int */
	signed long b4_61:2; 	/* UndCC_Violation even if long and int are the
							 * same size */
};
