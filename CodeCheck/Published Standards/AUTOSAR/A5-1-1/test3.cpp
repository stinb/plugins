// Allowed literal values

int helper(int v);

int test3_a(int v)
{
  int a;
  a = 0;            // UndCC_Violation(default)
  if (v == 0)       // UndCC_Violation(default)
    return 0;       // UndCC_Violation(default)
  return helper(0); // UndCC_Violation(default)
}

int test3_b(int v)
{
  return v + 5; // UndCC_Violation - only the listed values are allowed
}

int test3_c()
{
  int a;
  a = -1;  // UndCC_Violation(default) - a sign is not part of the literal
  a = 0U;  // UndCC_Violation - only the exact literal is allowed
  a = 0x0; // UndCC_Violation
  a = 0.0; // UndCC_Violation
  return a;
}
