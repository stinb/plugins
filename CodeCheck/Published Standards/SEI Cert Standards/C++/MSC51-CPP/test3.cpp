// Custom

#include <cstdlib>
#include <ctime>
#include <random>

#define ZERO() 0

#define SEED() dev()

std::random_device dev;

unsigned zero()
{
  return 0;
}

unsigned get_seed()
{
  return dev();
}

void generator_0()
{
  int x = 1 + 2;
  std::ranlux24_base engine(ZERO() + 3); // UndCC_Violation
}

void generator_1()
{
  std::ranlux48 engine(zero()); // UndCC_Violation
}

void generator_2()
{
  std::knuth_b engine(get_seed()); // UndCC_FalsePos
}

void generator_3()
{
  std::default_random_engine engine(dev()); // UndCC_Valid
  std::time_t t;
  engine.seed(std::time(&t)); // UndCC_Violation
  engine.seed(0);             // UndCC_Violation
  engine.seed();              // UndCC_Violation
  engine.seed(dev());         // UndCC_Valid
  engine.seed(SEED());        // UndCC_Valid
  engine.seed(get_seed());    // UndCC_FalsePos
  engine.seed(dev() * 0);     // UndCC_Violation
  engine.seed(dev() * 1);     // UndCC_FalsePos
}

void generator_4() {
  int x = 1 + 2;
  srand(ZERO() + 3); // UndCC_Violation
  rand();            // UndCC_FalsePos
}

void generator_5() {
  srand(zero()); // UndCC_Violation
  rand();        // UndCC_FalsePos
}

void generator_6() {
  srand(dev()); // UndCC_Valid
  rand();       // UndCC_FalsePos
}

void generator_7() {
  srand(get_seed()); // UndCC_FalsePos
  rand();            // UndCC_FalsePos
}

void generator_8() {
  rand(); // UndCC_Violation
  rand(); // UndCC_Violation
}
