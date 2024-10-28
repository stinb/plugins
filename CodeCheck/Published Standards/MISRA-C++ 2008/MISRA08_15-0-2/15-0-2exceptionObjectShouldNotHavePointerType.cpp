class A
{
// Implementation
};
void fn ( int i ) // UndCC_Violation
{
  static A a1;
  A * a2 = new A;
  if ( i > 10 )
  {
    throw &a1; 
  }
  else
  {
    throw a2; 
  }
}
