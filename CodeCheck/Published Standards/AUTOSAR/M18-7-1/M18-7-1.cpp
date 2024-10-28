// Custom

#include <cstdlib>
#include <cstdint>
#include <csignal> // UndCC_Violation
#include <iostream>

void my_handler ( int32_t );

void f1 ( )
{
  signal ( 1, my_handler );
  raise(1);
}

int raise()
{
  return 0;
}

int main()
{
  int val = raise();
  std::cout << val << '\n';

  return 0;
}
