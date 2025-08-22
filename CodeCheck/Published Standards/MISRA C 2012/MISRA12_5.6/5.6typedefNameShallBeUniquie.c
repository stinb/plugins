void func_56 ( void )
{
	{
		typedef unsigned char u8_t_56; 	/* UndCC_Violation - reused */
	}
	{
		typedef unsigned char u8_t_56;
	}
}

typedef float mass_56;
void func1_56 ( void )
{
	float mass_56 = 0.0f; 				/* UndCC_Violation - reuse */
}

typedef struct list_56
{
	struct list_56 *next_56;
	short element_56;
} list_56; 								/* UndCC_Valid - exception */

typedef struct
{
	struct chain_56                     /* UndCC_Violation */
	{
		struct chain_56 *list_56;       /* UndCC_Violation */
		short element_56;
	} s1_56;
	unsigned short length_56;
} chain_56;


typedef struct test_56;
