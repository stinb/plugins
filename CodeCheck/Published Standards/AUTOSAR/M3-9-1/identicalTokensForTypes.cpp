#include <cstdint>

typedef int32_t INT;
       INT     i;
extern int32_t i;    
       INT j;
extern INT j;        // UndCC_Valid
// The following lines break Rule 3-9-2
extern void f ( signed int );
       void f (        int );    // UndCC_Violation
extern void g ( const  int );
       void g (        int );    // UndCC_Violation

// extern short a;
// extern signed short b;
// extern short int c;
// extern signed short d;
// extern unsigned short e;
// extern unsigned short int f1;
// extern int g1;
// extern signed h;
// extern signed int i;
// extern unsigned j1;
// extern unsigned int k;
// extern long l;
// extern signed long m;
// extern long int n;
// extern signed long int o;
// extern unsigned long p;
// extern unsigned long int q;
