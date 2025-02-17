// Test from LMCO.
#include <stdexcept>
using std::logic_error;

class class1 {
public:
  class1(const char* msg) : msg_(msg) {}
  const char* what() const { return msg_; }

private:
  const char* msg_;
};

class1 ret_class1(char *);

void func1(int a) {
  if (a)
    ;
  else
    throw logic_error("error"); // possible violation, unknown return type // UndCC_Violation
}

void func2(int a) {
  throw ret_class1("error");	// okay, class return type
}

int ret_int1(char *);
int ret_int2(char *);
void func3(int a ) {
  throw ret_int1("error");	// UndCC_Violation, int return type
  throw ret_int2("error");	// possible violation, unknown return type // UndCC_Violation
}
