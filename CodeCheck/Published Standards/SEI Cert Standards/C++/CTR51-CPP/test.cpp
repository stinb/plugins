#include <deque>
  
void f(const double *items, std::size_t count) {
  std::deque<double> d;
  auto pos = d.begin();
  for (std::size_t i = 0; i < count; ++i, ++pos) {
    d.insert(pos, items[i] + 41.0); // UndCC_Violation
    pos = d.insert(pos, items[i] + 41.0); // UndCC_Valid
  }
}
