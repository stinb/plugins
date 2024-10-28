// Custom

// Length for C90
//  123456**********************************     Characters
int a_aaa0;
int a_aaa1;
int a_bbbb0; // UndCC_Violation(a) - same as the next 1
int a_bbbb1;

// Length for C999
//  1234567890123456789012345678901*********     Characters
int a_cccccccccccccccccccccccccccc0;  // UndCC_Violation(a) - same as the next 1
int a_cccccccccccccccccccccccccccc1;
int a_ddddddddddddddddddddddddddddd0; // UndCC_Violation(a,c) - same as the next 1
int a_ddddddddddddddddddddddddddddd1;

// Types
//          1234567890123456789012345678901*********     Characters
typedef int a_eeeeeeeeeeeeeeeeeeeeeeeeeeeee0_t; // UndCC_Violation(a,c) - same as the next 1
typedef int a_eeeeeeeeeeeeeeeeeeeeeeeeeeeee1_t;

// Functions
//   1234567890123456789012345678901*********     Characters
void a_fffffffffffffffffffffffffffff0_t() {} // UndCC_Violation(a,c) - same as the next 1
void a_fffffffffffffffffffffffffffff1_t() {}
