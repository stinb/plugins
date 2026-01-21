// Custom

enum Suit {
  Diamonds,
  Hearts,
  Clubs,
  Spades
};

union DATATYPE {                  // UndCC_Violation
  char   ch;
  int    i;
  long   l;
  float  f;
  double d;
};

struct LameStruct {
  union DATATYPE invalidMember;   // UndCC_Violation
  int validMember;
  enum Suit validEnumMember;
};

using invalid_t = union DATATYPE; // UndCC_Violation

struct LameStruct validObject;
enum Suit validEnumObject;
union DATATYPE invalidObject1;    // UndCC_Violation
invalid_t invalidObject2;         // UndCC_Violation

using test = union test {         // UndCC_Violation
  int i;
  double j;
};

void f(test t);                   // UndCC_Violation

test tests[3];                    // UndCC_Violation
