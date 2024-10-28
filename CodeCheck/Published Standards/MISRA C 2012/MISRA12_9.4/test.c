struct mystruct
{
  int a;
  int b;
  int c;
  int d;
};

struct mystruct s0 = { .a = 100, .b = -1, .a = 42, .d = 999 }; // UndCC_Violation
struct mystruct s1 = { .a = 100, .b = -1, .c = 42, .d = 999 }; // UndCC_Valid


union myunion {
  int a;
  int b;
  int c;
  int d;
};

union myunion s2 = { .a = 100, .b = -1, .c = 42, .d = 999 }; // UndCC_Valid
union myunion s3 = { .a = 100, .b = -1, .a = 42, .d = 999 }; // UndCC_Violation
