void okay_undefined1();
void okay_undefined2(int);

void okay1();
void okay2(int p1);
void okay3(int p1,int p2,int p3);
void okay4(int (*p1)(),int (*p2)(int,int));
void okay5(int p1,int);
void okay6(int,int p2);
void okay7(int p1,...);

struct A {
  void okay8(int p1);
  void okay9(int p1,int p2,int p3);
};
void okay10(int p1,char *p2);

void violation1(int); // UndCC_Violation
void violation2(int x1); // UndCC_Violation
void violation3(int p1,int x2,int p3); // UndCC_Violation
//void violation4(int (*p1)(),int (*x2)(int,int));


struct B {
  void violation4(int); // UndCC_Violation
  void violation5(int x1,int p2,int p3); // UndCC_Violation
};
