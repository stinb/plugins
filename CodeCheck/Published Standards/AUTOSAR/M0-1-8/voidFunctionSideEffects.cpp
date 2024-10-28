int a = 0;

// UndCC_Valid
void fun() {
  ++a;
}


void fun1() { // UndCC_Violation
  int b = 0;
  ++b;
}

// UndCC_Valid
void fun2() {
  fun();
}
