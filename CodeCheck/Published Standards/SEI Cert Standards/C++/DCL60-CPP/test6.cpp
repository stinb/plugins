// SEI CERT C++

const int n = 42;

int g6(int lhs, int rhs);

inline int f6(int k) {
  return g6(k, n); // UndCC_Valid
}
