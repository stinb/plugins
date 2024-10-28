// Custom

#if __cplusplus < 201703L // Before C++17
  #define and &&
  #define or ||
#endif // Before C++17

void symbol(int value)
{
  value > 0 && value < 3;     // UndCC_Violation
  (value > 1) && (value < 2); // UndCC_Valid
  value > 1 || (value < 2);   // UndCC_Violation
  (value > 1) || value < 2;   // UndCC_Violation
  value && value && value;    // UndCC_Valid - chain allowed by option
  value || value || value;    // UndCC_Valid - chain allowed by option
  value || value && value;    // UndCC_Violation
  value && value || value;    // UndCC_Violation
}

void keyword(int value)
{
  value > 0 and value < 3;     // UndCC_Violation
  (value > 1) and (value < 2); // UndCC_Valid
  value > 1 or (value < 2);    // UndCC_Violation
  (value > 1) or value < 2;    // UndCC_Violation
  value and value and value;   // UndCC_Valid - chain allowed by option
  value or value or value;     // UndCC_Valid - chain allowed by option
  value or value and value;    // UndCC_Violation
  value and value or value;    // UndCC_Violation
}

void symbol_and_keyword(int value)
{
  value and value && value; // UndCC_Valid - chain allowed by option
  value && value and value; // UndCC_Valid - chain allowed by option
  value or value || value;  // UndCC_Valid - chain allowed by option
  value || value or value;  // UndCC_Valid - chain allowed by option
  value or value && value;  // UndCC_Violation
  value || value and value; // UndCC_Violation
  value and value || value; // UndCC_Violation
  value && value or value;  // UndCC_Violation
}
