class A {
  public: struct B { int c; } b;
} a,*aa=&a;
class A::B ab;
int d;
A afunc(),*aafunc();
int bfunc();

void okay1() { throw a; }
void okay2() { throw ab; }
void okay3() { throw a.b.c; }
void okay4() { throw aa->b.c; }
void okay5() { throw afunc(); }
void okay6() { throw ::a; }
void okay7() { throw *aa; }

void violation1() { throw new A; } // UndCC_Violation
void violation2() { throw new A::B; } // UndCC_Violation
void violation3() { throw new ::A; } // UndCC_Violation
void violation4() { throw aa; } // UndCC_Violation
void violation5() { throw 10; } // UndCC_Violation
void violation6() { throw "a"; } // UndCC_Violation
void violation7() { throw 'a'; } // UndCC_Violation
void violation8() { throw aafunc(); } // UndCC_Violation
void violation9() { throw &a; } // UndCC_Violation
void violation10() { throw ::aa; } // UndCC_Violation
void violation11() { throw bfunc(); } // UndCC_Violation
void violation12() { throw d; } // UndCC_Violation
