class A {};
class B {};
void f ( int i ) throw ( )
{
  try
  {
    if ( i > 10 )
    {
      throw A ( ); // UNDCC_Valid
    }
    else
    {
      throw B ( ); // UNDCC_Violation
    }
  }
  catch ( A const & )
  {
  }
}
