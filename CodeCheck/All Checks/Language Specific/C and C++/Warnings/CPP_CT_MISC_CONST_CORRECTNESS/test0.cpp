// https://releases.llvm.org/18.1.0/tools/clang/tools/extra/docs/clang-tidy/checks/misc/const-correctness.html

void analyze_values()
{
  int i_1 = 42;                                 // UndCC_Violation
  int const i_2 = 42;                           // UndCC_Valid

  int a_1[] = {42, 42, 42};                     // UndCC_Violation
  int const a_2[] = {42, 42, 42};               // UndCC_Valid
}

void analyze_references(int i)
{
  int& ref_1 = i;                               // UndCC_Violation
  int const& ref_2 = i;                         // UndCC_Valid
}

void warn_pointers_as_values(int value)
{
  const int * pointer_variable_1 = &value;      // UndCC_Violation
  const int *const pointer_variable_2 = &value; // UndCC_Valid
}
