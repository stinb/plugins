// M6-5-5.cpp

bool test_a ( bool * pB )
{
  *pB = (1!=1) ? true : false;
  return *pB;
}

bool test (int v)
{
  return (v > 10) ? true : false;
}

void f()
{
  int x;
  bool bool_a = false;
  for ( x = 0; ( x < 10 ) && !bool_a; ++x )
  {
    if ( true )
    {
      bool_a = true; // UndCC_Valid
    }
  }

  for ( x = 0;
  ( x < 10 ) && test_a ( &bool_a ); // UndCC_Violation
  ++x ) 
  { 
    // ... 
  }
  
  volatile bool status;
  for ( x = 0; ( x < 10 ) && status; ++x) // UndCC_Valid
  { }
  
  for ( x = 0; x < 10; bool_a = test( ++x ) ) // UndCC_Violation
  { }
  
  int y = 0;
  for ( x = 0; x < 10; x = y++ ) // UndCC_Violation
  { 

  }
  
  for(int x =0, y=10; x>y; x++, y--) // UndCC_Valid
  {
    
  }

}
