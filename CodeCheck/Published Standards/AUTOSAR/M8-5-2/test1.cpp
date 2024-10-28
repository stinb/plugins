// Custom

class C {};
struct S {};

C classes_a[3] = { C(), C(), C() };
C classes_b[3] = { C { }, C { }, C { } };
C classes_c[3] = { C { } }; // UndCC_FalseNeg

S structs_a[3] = { S(), S(), S() };
S structs_b[3] = { S { }, S { }, S { } };
S structs_c[3] = { S { } }; // UndCC_FalseNeg

int no_ents_3[3] = { 1, 2, 3 };
int no_ents_5[5] = { 1, 2, 3 }; // UndCC_Violation

// int nums[5] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; // Compilation error
