// SEI CERT C++

enum Constants {
  N = 42
};

int g7(const int &lhs, const int &rhs);

inline int f7(int k) {
  return g7(k, N); // UndCC_Valid
}
