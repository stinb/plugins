int a = 1;

class E
{
  public:
    E ( ) { }  // UndCC_Valid - Assume constructor cannot cause an exception
};

void myFunction() {
  try 
  {
    if ( a == 1 ) 
    { 
      throw E ( );  // Compliant – no exception thrown when constructing the object
    }
  }
  catch (...) {}
}

// construction of E2 causes an exception to be thrown
class E2
{
  public:
    E2 ( )  // UndCC_Violation - E2 throws an int exception
    {
      throw 10;
    } 
};

void myFunction2() {
  try 
  {
    if ( a == 1 )
    {
      throw E2 ( );  // Non-compliant – int exception thrown
    } 
  }
  catch (...) {}
}
