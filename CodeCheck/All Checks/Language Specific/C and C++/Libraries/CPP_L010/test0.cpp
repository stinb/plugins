// MISRA C++ 2008

#include <setjmp.h>

void f2 ( );

jmp_buf buf;

void f1 ( )
{
   if ( !setjmp ( buf ) )   // UndCC_Violation
   {
      f2 ( );
   }
   else
   {
   }
}

void f2 ( )
{
   longjmp ( buf, 10 );     // UndCC_Violation
}
