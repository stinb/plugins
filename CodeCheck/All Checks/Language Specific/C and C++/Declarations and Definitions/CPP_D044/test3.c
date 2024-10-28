// Custom

int fun(int a) {
  return a;
}

struct myStruct {
  int num; // UndCC_Valid - Member variable
};

int main() {
  struct myStruct s; // UndCC_Valid - Used in this scope
  s.num = 5;
  struct myStruct s0;

  struct myOtherStruct {
    int otherNum; // UndCC_Valid - Member variable
  };

  struct myOtherStruct s1;
  s1.otherNum = 3;
  if (1) {
    int a = s1.otherNum;
  }

  int i;      // UndCC_Valid - Valid because used in 'for' condition before {
  int b = 10; // UndCC_Valid - Valid because used in 'for' condition before {
  int c;      // UndCC_Valid - Exception because the immediate lower scope is a loop
  for (i = 0; i < b; i++) {
    c = i;
  }

  int d = 3; // UndCC_Valid - Valid because used in call, not in {
  int e = fun(d);

  int f = 4; // UndCC_Violation - Only used in 1 lower scope
  if (1) {
    int g = f;
  }

  int unused; // Unclear - This rule doesn't describe what to do here, but it
              // doesn't matter, as it's a violation of Rule 10.
}
