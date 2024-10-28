int myInt = 1;

void test () {
throw 10;
}

class E
{
public:
  E ( )
  {
    // Do something here
  }   // Assume constructor cannot cause an exception
};

// construction of E2 causes an exception to be thrown
class E2
{
public:
  E2 ( )
  {
    test();
  }
};

void a() {
  try
  {
    if( myInt == 1 )
    {
      test(); // UndCC_Valid - no exception thrown
    }              // when constructing the object
  }
  catch (int) {}
}

void b() {
  try
  {
    if( myInt == 1 )
    {
      throw E2(); // UndCC_Violation - int exception thrown
    }               // when constructing the object
  }
  catch (int) {}
}
