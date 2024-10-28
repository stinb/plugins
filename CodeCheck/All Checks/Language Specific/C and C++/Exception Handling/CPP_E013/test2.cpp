// Test from LMCO.
#include <stdexcept>
using std::logic_error;

void func1(int a) {
  if (a)
    ;
  else
    throw logic_error("error"); // possible violation, unknown return type // UndCC_Violation
}

class class1 ret_class1(char *);
void func2(int a) {
  throw ret_class1("error");	// okay, class return type
}

int ret_int1(char *);
void func3(int a ) {
  throw ret_int1("error");	// UndCC_Violation, int return type
  throw ret_int2("error");	// possible violation, unknown return type // UndCC_Violation
}

