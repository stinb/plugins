// MISRA C++ 2008

typedef char char_t;

void f ( const char_t * p )
{
   *const_cast< char_t * >( p ) = '\0';   // UndCC_Violation
}
int main ( )
{
   f ( "Hello World!" );
}
