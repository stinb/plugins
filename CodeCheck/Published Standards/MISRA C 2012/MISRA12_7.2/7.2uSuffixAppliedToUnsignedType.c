#define testMacro1_72 32767
#define testMacro2_72 32768
#define testMacro3_72 2147483647
#define testMacro4_72 2147483648 	 // UndCC_Violation

#if 32768 == 0x8000					 // UndCC_Violation

#elif 0xff000000 == 7 				 // UndCC_Violation

#endif

int	a_72	= 32767;				/* UndCC_Valid */
int	b_72	= 0x7fff;				/* UndCC_Valid */
long c_72	= 32768;				/* UndCC_Valid */
int d_72	= 32768u;				/* UndCC_Valid */
int e_72	= 0x8000;				/* UndCC_Violation */
int f_72	= 0x8000u;				/* UndCC_Valid */
