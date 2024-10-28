// MISRA C++ 2008

extern void f ( void );

void g()
{
  if ( 0 == f )           // UndCC_Violation
  {
    // ...
  }
  void (*p)( void ) = f;  // UndCC_Violation
  if ( 0 == &f )          // UndCC_Valid
  {
    (f)();                // UndCC_Valid as function is called
  }
}

void h()
{
  void (*p)( void ) = &f; // UndCC_Valid
}
