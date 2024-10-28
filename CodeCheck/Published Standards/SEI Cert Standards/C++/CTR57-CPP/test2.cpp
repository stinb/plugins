// SEI CERT C++

#include <iostream>
#include <set>

class S {
  int i, j;

public:
  S(int i, int j) : i(i), j(j) {}

  friend bool operator<(const S &lhs, const S &rhs) { // UndCC_FalseNeg
    return lhs.i < rhs.i && lhs.j < rhs.j;
  }

  friend std::ostream &operator<<(std::ostream &os, const S& o) {
    os << "i: " << o.i << ", j: " << o.j;
    return os;
  }
};

void f() {
  std::set<S> t{S(1, 1), S(1, 2), S(2, 1)};
  for (auto v : t) {
    std::cout << v << std::endl;
  }
}
