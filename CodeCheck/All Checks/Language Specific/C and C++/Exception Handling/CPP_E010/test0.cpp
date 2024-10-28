// MISRA C++ 2008 - Rule 15-4-1

using char_t = char;

// Translation unit A
void foo( ) throw ( const char_t * ) // UndCC_Violation
{
  throw "Hello World!";
}
