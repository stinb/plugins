// SEI CERT C++

struct alignas(32) Vector {
  char elems[32];
};

Vector *f() {
  Vector *pv = new Vector; // UndCC_Violation(Win)
  return pv;
}
