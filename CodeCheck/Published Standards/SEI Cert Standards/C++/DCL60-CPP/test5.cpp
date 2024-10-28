// SEI CERT C++

const int n = 42;

int g5(const int &lhs, const int &rhs);

inline int f5(int k) {
  return g5(k, n); // UndCC_Violation
}
