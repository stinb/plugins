extern void f_844 ( void );

int main_844 () {

  if ( 0 == f_844 ) // UndCC_Violation
  {
    // ...
  }
  void (*p_844)( void ) = f_844; // UndCC_Violation
  if ( 0 == &f_844 ) // UndCC_Valid
  {
    (f_844)(); // UndCC_Valid as function is called
  }
  void (*q_844)( void ) = &f_844; // UndCC_Valid

  return 0;
}
