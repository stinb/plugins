// Custom

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

void test_1a()
{
  char a;                   // UndCC_Violation(allow_size_t,strict)
  signed char b;            // UndCC_Violation(allow_size_t,strict)
  unsigned char c;          // UndCC_Violation(allow_size_t,strict)
  short d;                  // UndCC_Violation
  short int e;              // UndCC_Violation
  signed short f;           // UndCC_Violation
  signed short int g;       // UndCC_Violation
  unsigned short h;         // UndCC_Violation
  unsigned short int i;     // UndCC_Violation
  int j;                    // UndCC_Violation
  signed k;                 // UndCC_Violation
  signed int l;             // UndCC_Violation
  unsigned m;               // UndCC_Violation
  unsigned int n;           // UndCC_Violation
  long o;                   // UndCC_Violation
  long int p;               // UndCC_Violation
  signed long q;            // UndCC_Violation
  signed long int r;        // UndCC_Violation
  unsigned long s;          // UndCC_Violation
  unsigned long int t;      // UndCC_Violation
  long long u;              // UndCC_Violation
  long long int v;          // UndCC_Violation
  signed long long w;       // UndCC_Violation
  signed long long int x;   // UndCC_Violation
  unsigned long long y;     // UndCC_Violation
  unsigned long long int z; // UndCC_Violation
}

void test1_b(int *a) // UndCC_Violation
{
  bool b;        // UndCC_Valid? - Not considered a number, so probably allowed
  wchar_t c;     // UndCC_Valid - Allowed by exception
  size_t d;      // UndCC_Violation(allow_char,strict)
  std::size_t e; // UndCC_Violation(allow_char,strict)
  int32_t f;
  std::int32_t g;
}

void test1_c()
{
  std::pair<int32_t, int32_t> a;
  std::pair<int32_t, size_t> b;  // UndCC_Violation(allow_char,strict)
  std::pair<int32_t, int> c;     // UndCC_Violation
}

void test1_d(int32_t *a)
{
  std::ofstream b; // UndCC_Violation(win(allow_size_t,strict),lin(allow_size_t,strict)) - std::basic_ofstream<char>
  std::string c;   // UndCC_Violation(win(allow_size_t,strict),lin(allow_size_t,strict)) - std::basic_string<char>
}

void test1_e(const std::vector<int32_t> &nums)
{
  for (const int32_t x : nums)
    for (int32_t y : nums)
      ;
}

void test1_f(const std::vector<float> &nums)
{
  for (const float x : nums)
    for (float y : nums)
      ;
}
