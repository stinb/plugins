
#include <iostream>
#include <string>
#include "animal.hpp"

int main() {
  
  
  Animal mammal_01;
  Animal mammal_02(7, "Joe", "Rhino");
  Animal reptile_01;
  
  // Set attributes
  mammal_01.set_age(6);
  mammal_01.set_name("Gary");
  mammal_01.set_species("Monkey");
  
  // Print attribute values
  std::cout << mammal_01.get_name() << " is a " << mammal_01.get_age() << " year old " << mammal_01.get_species() << std::endl;
  std::cout << reptile_01.get_name() << " is a " << reptile_01.get_age() << " year old " << reptile_01.get_species() << std::endl;
  
  return 0;
}
