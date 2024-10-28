// Custom

class Exception { };

void foobar ( )
{
  throw 21;
}

void foobarish ()
{
  throw 10;
}

void foo ( )
{
  foobar( );     // UndCC_Violation - foo throws int here
}

void goo ( ) throw ( Exception )
{
  foo ( );       // UndCC_Violation - goo throws int here
  try {
    foobarish(); // UndCC_Violation - goo throws int here
  }
  catch ( ... ) {
    throw 10;
  }
}
