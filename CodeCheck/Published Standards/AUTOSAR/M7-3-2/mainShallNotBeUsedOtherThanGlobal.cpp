int main ( ) // UndCC_Valid
{
	return 0;
}
namespace
{
  int main ( ) // UndCC_Violation
  {
    return 9;
  }
}
namespace NS
{
  int main ( ) // UndCC_Violation
  {
	return 0;
  }
}
