// Custom

#define testMacro1 32767
#define testMacro2 32768
#define testMacro3 2147483647
#define testMacro4 2147483648 // UndCC_Violation

#if 32768 == 0x8000           // UndCC_Valid - both longs in #if or #elif

#elif 0xff000000 == 7         // UndCC_Violation

#endif
