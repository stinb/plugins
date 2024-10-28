
#include <string>
#include <random>

#include "test02-m.h"

using namespace std;

void f00() {
  std::string id("ID"); // Holds the ID, starting with the characters "ID" followed
  // by a random integer in the range [0-10000].
  id += std::to_string(std::rand() % 10000); // UNDCC_Violation
  
  int x = rand() % 10000; // UNDCC_Violation
}


void f01() {
  std::string id("ID"); // Holds the ID, starting with the characters "ID" followed
  // by a random integer in the range [0-10000].
  std::uniform_int_distribution<int> distribution(0, 10000);
  std::random_device rd;
  std::mt19937 engine(rd());
  id += std::to_string(distribution(engine)); // UNDCC_Valid
  // ...
}
