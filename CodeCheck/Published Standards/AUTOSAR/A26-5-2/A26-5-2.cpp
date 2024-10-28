// $Id: A26-5-2.cpp 311495 2018-03-13 13:02:54Z michal.szczepankiewicz $

#include <iostream>
#include <random>

int main()
{
  std::random_device rd;
  std::default_random_engine eng{rd()}; // UndCC_Valid
  std::uniform_int_distribution<int> ud{0, 100};
  int r1 = ud(eng);
  std::cout << "Random value using std::random_device: " << r1 << std::endl;


  std::default_random_engine eng2{};  // UndCC_Violation
  std::uniform_int_distribution<int> ud2{0, 100};
  int r2 = ud2(eng);
  std::cout << "Random value using std::random_device: " << r2 << std::endl;

  return 0;
}
