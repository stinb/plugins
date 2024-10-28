int32_t main( )
{
  try
  {
    // program code
  }
  catch ( ... ) // UndCC_Valid
  {
    // Handle unexpected exceptions
  }
  return 0;
}

int32_t wmain( ) // UndCC_Violation
{
	return 0;  
}
