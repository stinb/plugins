/* Custom - UndCC_Valid */

// UndCC_Violation
int main()
{
  int i; /* UndCC_Valid // another comment */

  // UndCC_Violation

  // UndCC_Valid - considered part of the above comment

  printf("// valid string masquarading as a comment\n");
  return 0;
}
