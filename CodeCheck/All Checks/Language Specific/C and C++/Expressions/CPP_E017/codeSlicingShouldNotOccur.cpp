 struct A
  {
    A(int a) : a_var(a) {}
    int a_var;
  };
   
  struct B : public A
  {
    B(int a, int b) : A(a), b_var(b) {}
    int b_var;
  };
   
  B &getB()
  {
    static B b(1, 2);
    return b;
  }
   
  int main()
  {
    // Normal assignment by value to a
    A a(3);
    a = getB(); // UndCC_Violation
    // a.a_var == 1, b.b_var not copied to a
   
    B b2(3, 4);
    A &a2 = b2;
    // Partial assignment by value through reference to b2
    a2 = getB(); // UndCC_Violation
    // b2.a_var == 1, b2.b_var == 4!
   
    return 0;
  }