// Custom

#include <iostream>
#include <string>

std::string global;

void a(std::string local)
{
  std::cout << local << '\n';
}

void b(int x)
{
  std::string local;
  if (x) {
    a(std::move(local)); // UndCC_Valid
  }
}

void c(int x)
{
  std::string local;
  if (x) {
    a(std::move(local)); // UndCC_Violation
  }
  std::cout << local << '\n';
}

void d()
{
  a(std::move(global)); // UndCC_Violation
}

void e()
{
  for (unsigned i = 0; i < 10; ++i) {
    std::string local(1, static_cast<char>('0' + i));
    for (unsigned j = i; j < 10; ++j) {
      local.append(1, static_cast<char>('0' + j));
    }
    a(std::move(local)); // UndCC_Valid
  }
}
