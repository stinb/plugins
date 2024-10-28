typedef int *type1;
typedef type1 type2;

void okay1() {}
void okay2(int p1,char p2) {}
void okay3(int *p1) { if (p1) *p1; }
void okay4(int *p1) { int a; p1 = &a; *p1; }
void okay5(type1 p1) { if (p1) *p1; }
void okay6(type2 p1) { if (p1) *p1; }

void violation1(int *p1) { *p1; } // UndCC_Violation
void violation2(type1 p1) { *p1; } // UndCC_Violation
void violation3(type2 p1) { *p1; } // UndCC_Violation
void violation4(int *p1) {
*p1; // UndCC_Violation
*p1; // UndCC_Violation
*p1; // UndCC_Violation
p1=0;
*p1; // not violation
}
