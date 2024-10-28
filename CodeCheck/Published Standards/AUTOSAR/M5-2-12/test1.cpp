// Custom

#if __cplusplus >= 202002L // C++20

#include <span>

void receive_ptr(char *string);
void receive_span(std::span<char> string);

void f()
{
  char string[3];
  receive_ptr(string);  // UndCC_Violation(strict,whitelist) - Dimension "3" lost due to array to pointer conversion
  receive_span(string); // UndCC_Valid - Dimension preserved
}

#endif // C++20
