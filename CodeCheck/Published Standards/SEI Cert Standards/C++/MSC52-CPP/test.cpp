#include <vector>
int absolute_value(int a) {
if (a < 0) {
  return -a;
}
} // UndCC_Violation
int absolute_value1(int a) {
  if (a < 0) {
    return -a;
  }
  return a;
}

std::size_t f(std::vector<int> &v, std::size_t s) try {
  v.resize(s);
  return s;
} catch (...) {
} // UndCC_Violation

std::size_t f1(std::vector<int> &v, std::size_t s) try {
  v.resize(s);
  return s;
} catch (...) {
  return 0;
}
