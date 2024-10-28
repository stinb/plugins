// Custom

void fn_no_definition(char *string);
void fn_definition(char *string) {}

void f()
{
  char string[3];
  fn_no_definition(string); // UndCC_Violation(strict,whitelist) - Dimension "3" lost due to array to pointer conversion
  fn_definition(string);    // UndCC_Violation(strict,allow_unresolved) - Dimension "3" lost due to array to pointer conversion
}
