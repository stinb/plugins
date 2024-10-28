
#ifndef ANIMAL_HPP
#define ANIMAL_HPP
#include <string>

class Animal
{
private:
  int d_age; // this is a valid naming convention for this data member
  std::string name_d_; // this should flag violation, naming convention must be a PREFIX // UndCC_Violation
  std::string species;  // UndCC_Violation
  
public:
  Animal();
  int m_get_age(); // valid naming convention
  void m_set_age(int); // valid naming convention
  std::string get_name(); // UndCC_Violation
  void set_name(std::string); // UndCC_Violation
  std::string get_species(); // UndCC_Violation
  void set_species(std::string); // UndCC_Violation
};

#endif
