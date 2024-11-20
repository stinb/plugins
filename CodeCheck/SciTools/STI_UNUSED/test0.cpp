int functionWithSideEffect();


void good()
{
  int i = 0;
  (void) i; // suppress violation

  (void) functionWithSideEffect(); // discard return value
}


void bad()                          // UndCC_Violation
{
  int i = 0;                        // UndCC_Violation

  int j = functionWithSideEffect(); // UndCC_Violation
}


int main()
{
  good();
}
