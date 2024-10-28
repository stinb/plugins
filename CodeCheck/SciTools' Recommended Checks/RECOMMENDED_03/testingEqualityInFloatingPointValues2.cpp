void okay1() {
  int a,b;
  float c;
  a==b;
  a==1;
  1==a;
  a==0xe0;
  a==(int)c;
  (int)c==a;
}


void violation1() {
  int a;
  float b;
  double c;
  a==b;   // UndCC_Violation
  a==c;   // UndCC_Violation
  b==c;   // UndCC_Violation
  a==1.0; // UndCC_Violation
  1.0==a; // UndCC_Violation
  a==1.;  // UndCC_Violation
  a==1e1; // UndCC_Violation
}


struct struct1 {
  float b;
};

void limitation1() {
  int a;
  float b;
  struct1 c;
  a==1+b;       // UndCC_Violation
  a==c.b;       // UndCC_Violation
  (int) c.b==0; // UndCC_Valid
}
