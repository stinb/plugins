#include <iostream>
#include <string>

void f(const std::string &input) {
  std::string email;
  
  // Copy input into email converting ";" to " "
  std::string::iterator loc = email.begin();
  for (auto i = input.begin(), e = input.end(); i != e; ++i, ++loc) {
    loc = email.insert(loc, *i != ';' ? *i : ' '); // UndCC_Valid
  }
}

void f1(const std::string &input) {
  std::string email;
  
  // Copy input into email converting ";" to " "
  std::string::iterator loc = email.begin();
  for (auto i = input.begin(), e = input.end(); i != e; ++i, ++loc) {
    email.insert(loc, *i != ';' ? *i : ' '); // UndCC_Violation
  }
}

extern void g(const char *);

void f2(std::string &exampleString) {
  const char *data = exampleString.data(); // UndCC_Violation
  // ...
  exampleString.replace(0, 2, "bb");
  // ...
  g(data);
}


void f3(std::string &exampleString) {
  // ...
  exampleString.replace(0, 2, "bb");
  // ...
  g(exampleString.data()); // UndCC_Valid
}

