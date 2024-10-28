// Custom

#include <array>

// Template parameters: type
template <typename T>
void templates_a() {}
template <typename T>
struct TemplatesB
{
#ifdef _WIN32
  template <typename T> // UndCC_Violation(WIN)
  T templates_b(T x)
  {
    return x;
  }
#endif
};


// Template parameters: object
class Foo {
  template <std::size_t SIZE>
  void foo(const std::array<int, SIZE>& ints);
};
template <std::size_t SIZE>
void Foo::foo(const std::array<int, SIZE>& ints)
{

}
// Label identifiers
int a;
int b;
void labels()
{
a:
b:
c:
  int c;
}

// Tag identifiers
class d;
struct e;
union f;
enum g {};
int h;
int i;
int j;
int k;
void tags()
{
  int d;
  int e;
  int f;
  int g;
  class h {};
  struct i {};
  union j {};
  enum k {};
}

// Member identifiers
struct l {
  int m;
};
int n;
void members()
{
  int l;
  struct m {
    int n;
  };
}

// Ordinary identifiers
int o;
void p();
typedef int q;
using r = int;
enum { s };
int t;
int u;
int v;
void ordinary(int o) // UndCC_Violation
{
  int p;         // UndCC_Violation
  int q;         // UndCC_Violation
  int r;         // UndCC_Violation
  int s;         // UndCC_Violation
  typedef int t; // UndCC_Violation
  using u = int; // UndCC_Violation
  enum { v };    // UndCC_Violation
}

// Ordinary identifiers in lambda functions
void lambdas()
{
  int w;
  int x;
  [w]() {
    int w; // UndCC_Violation
    int x;
    []() {
      int w;
      int x;
    };
    [w]() {
      int w; // UndCC_Violation
      int x;
    };
    [x]() {
      int w;
      int x; // UndCC_Violation
    };
  };
}

// Ordinary identifiers in parameters
void parameters_in_definition(int w)
{
  {
    int w; // UndCC_Violation
  }
  [](int w) {

  };
}
int w;
void parameters_in_declaration(int x);
int x;

// Ordinary identifiers in control-flow conditions
void control_flow()
{
  for (int y = 0; y < 5; y++) {

  }
  for (int y = 0; y < 5; y++)
    ;
  while (int y = 0) {

  }
  while (int y = 0)
    ;
  if (int y = 0) {

  }
  if (int y = 0)
    ;
  switch (int y = 0) {

  }
  switch (int y = 0)
    ;
}

// Ordinary identifiers repeated
void y(int z);
void y(int z)
{

}
void z();
typedef int A(int C);
typedef int B(int C);
int (*C)(int E);
int (*D)(int E);
int E(int F);
int E(long G);

// Macros
#define FN_DEC(name) void name(int H);
FN_DEC(F)
FN_DEC(G)
#define FN_DEF(name) void name() { int J; }
FN_DEF(H)
FN_DEF(I)
int J;
#define J J
