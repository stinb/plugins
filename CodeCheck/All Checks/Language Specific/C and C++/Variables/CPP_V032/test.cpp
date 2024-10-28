int main() {
  struct S1 { int a; } s1;
  struct S2 { int a; private: int b; } s2;
  union U { int a; double b; } u = {0};
  int arr[2];
  
  int* p1 = reinterpret_cast<int*>(&s1); // UndCC_Violation
  int* p2 = reinterpret_cast<int*>(&s2); // UndCC_Violation
  int* p3 = reinterpret_cast<int*>(&u); // UndCC_Violation
  double* p4 = reinterpret_cast<double*>(p3); // UndCC_Violation
  int* p5 = reinterpret_cast<int*>(&arr); // UndCC_Violation
}
