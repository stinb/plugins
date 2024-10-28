#define a 0U; 
#define b 0u; // UndCC_Violation
#define c 0L;
#define d 0l; // UndCC_Violation
#define e 0UL;
#define f 0Ul; // UndCC_Violation
#define g 0x12bU;
#define h 0x12bu; // UndCC_Violation
#define m 1.2F;
#define n 2.4f; // UndCC_Violation
#define o 1.2L;
#define p 2.4l; // UndCC_Violation
#define q o;
#define r p;


int main() {

   return 0;
}
