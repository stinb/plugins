int main()
{
  int a1 = 40;
  const volatile int* b1 = &a1;
  int* c1 = const_cast <int *> (b1); // UndCC_Violation
  
  int a2 = 40;
  const int* b2 = &a2;
  int* c2 = const_cast <int *> (b2); // UndCC_Valid
  return 0;
}
