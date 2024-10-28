wchar_t a = 'a';  // UndCC_Valid
wchar_t b = '\r'; // UndCC_Valid
wchar_t c = 10;   // UndCC_Violation
char d = 'd';     // UndCC_Valid with this rule, but breaks Rule 3–9–2

typedef int foo;
foo number = 10;
wchar_t e = number; // UndCC_Violation
