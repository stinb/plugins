// SEI CERT C++ - CTR54-CPP

#include <iostream>
#include <iterator>
#include <vector>

template <typename RandIter>
bool in_range_impl(RandIter test, RandIter r_begin, RandIter r_end, std::random_access_iterator_tag) {
  return test >= r_begin && test < r_end; // UndCC_Violation
}

template <typename Iter>
bool in_range(Iter test, Iter r_begin, Iter r_end) {
  typename std::iterator_traits<Iter>::iterator_category cat;
  return in_range_impl(test, r_begin, r_end, cat);
}

void f() {
  std::vector<double> foo(10);
  std::vector<double> bar(1);
  std::cout << std::boolalpha << in_range(bar.begin(), foo.begin(), foo.end());
}
