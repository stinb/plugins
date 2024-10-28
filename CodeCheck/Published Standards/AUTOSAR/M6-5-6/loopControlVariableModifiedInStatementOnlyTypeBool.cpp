int fn()
{
  return 1;
}

void f1()
{
  int x = 0;
  int u8a, uint8_a;
  bool flag = true;

  for ( x = 0; ( x < 10 ) && flag; ++x )             // UndCC_Valid
  {
    u8a = fn ( );
    flag = u8a != 3U;
  }

  for ( x = 0; ( x < 10 ) && ( u8a != 3U ); ++x )    // UndCC_Violation
  {
    uint8_a = fn ( );
  }

}

void f2()
{
  int x;
  int u8a, uint8_a;
  bool flag = true;
  int iMaxVal = 10;

  x = 0;
  while (( iMaxVal > x ) && flag)                    // UndCC_Valid
  {
    u8a = fn ( );
    flag = u8a != 3U;
    ++x;
  }

  x = 0;
  while ( ( x < 10 ) && ( u8a != 3U ))               
  {
    uint8_a = fn ( );
    ++x;
  }
}

void f3()
{
  int x;
  int u8a, uint8_a;
  bool flag = false;
  int iMaxVal = 10;

  x = 0;
  do
  {
    u8a = fn ( );
    flag = u8a != 3U;
    ++x;
  } while (true && ( x < 10 ));                      // UndCC_Valid

  x = 0;
  do
  {
    uint8_a = fn ( );
    ++x;
  } while ( ( x < iMaxVal ) && ( u8a != 3U ));       
}
