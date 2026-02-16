// Custom

void test_class()
{
  typedef class C0_tag {} C0; // UndCC_Violation

  using C1 = class C1_tag {}; // UndCC_Violation

  class C2 {};                // UndCC_Violation

  class C3 {} c3;             // UndCC_Violation

  class C4 {};                // UndCC_Valid
  C4 c4;

  class C5 {};                // UndCC_Valid
  sizeof(C5);

  class C6 {                  // UndCC_Valid
  public:
    static void method();
  };
  C6::method();

  class C7 {};                // UndCC_Valid
  class C8: public C7 {};     // UndCC_Valid
  new C8;
}

void test_enum()
{
  typedef enum E0_tag {} E0; // UndCC_Violation

  using E1 = enum E1_tag {}; // UndCC_Violation

  enum E2 {};                // UndCC_Violation

  enum E3 {} E3;             // UndCC_Violation

  enum E4 {};                // UndCC_Valid
  E4 e4;

  enum E5 {};                // UndCC_Valid
  sizeof(E5);
}

void test_struct()
{
  typedef struct S0_tag {} S0; // UndCC_Violation

  using S1 = struct S1_tag {}; // UndCC_Violation

  struct S2 {};                // UndCC_Violation

  struct S3 {} s3;             // UndCC_Violation

  struct S4 {};                // UndCC_Valid
  S4 s4;

  struct S5 {};                // UndCC_Valid
  sizeof(S5);

  struct S6 {                  // UndCC_Valid
  public:
    static void method();
  };
  S6::method();

  struct S7 {};                // UndCC_Valid
  struct S8: public S7 {};     // UndCC_Valid
  new S8;
}

void test_union()
{
  typedef union U0_tag {} U0; // UndCC_Violation

  using U1 = union U1_tag {}; // UndCC_Violation

  union U2 {};                // UndCC_Violation

  union U3 {} u3;             // UndCC_Violation

  union U4 {};                // UndCC_Valid
  U4 u4;

  union U5 {};                // UndCC_Valid
  sizeof(U5);

  union U6 {                  // UndCC_Valid
  public:
    static void method();
  };
  U6::method();

  union U7 {};                // UndCC_Valid
  new U7;
}
