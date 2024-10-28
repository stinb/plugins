// Custom

#include <iostream>
#include <string>

using namespace std;

string global;

void a(string local)
{
  cout << local << '\n';
}

void b(int x)
{
  string local;
  if (x) {
    a(move(local)); // UndCC_Valid
  }
}

void c(int x)
{
  string local;
  if (x) {
    a(move(local)); // UndCC_Violation
  }
  cout << local << '\n';
}

void d()
{
  a(move(global)); // UndCC_Violation
}

void e()
{
  for (unsigned i = 0; i < 10; ++i) {
    string local(1, static_cast<char>('0' + i));
    for (unsigned j = i; j < 10; ++j) {
      local.append(1, static_cast<char>('0' + j));
    }
    a(move(local)); // UndCC_Valid
  }
}
