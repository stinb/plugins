int fun(const int &a) { /* UNDCC_Valid */
  return a;
}

int fun1(int a) { /* UNDCC_Violation */
  return a;
}
