class A {};
struct B {};
union C {};
typedef int Type1;
typedef A Type2;

int okay1() {}
int *okay2() {}
int &okay3() {}

Type1 okay4() {}
Type1 *okay5() {}
Type1 &okay6() {}

A *okay7() {}
A &okay8() {}

Type2 *okay9() {}
Type2 &okay10() {}

B *okay11() {}
C *okay12() {}

A violation1() {} // UndCC_Violation
Type2 violation2() {} // UndCC_Violation
B violation3() {} // UndCC_Violation
C violation4() {} // UndCC_Violation
