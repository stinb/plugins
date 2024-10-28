// SEI CERT C++

#include <functional>
#include <iostream>
#include <string>

class MyString {
  std::string data;

public:
  MyString(const std::string &data) : data(data) {}

  const std::string &get_data() const { return data; }
};

struct my_plus : std::binary_function<std::string, MyString, std::string> {
  std::string operator()(const std::string &lhs, const MyString &rhs) const {
    return lhs + rhs.get_data();
  }
};

void f() {
  std::string s1("My String");
  MyString s2(" + Your String");
  my_plus p;

  std::cout << p(s1, s2) << std::endl;
}
