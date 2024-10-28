
#ifndef ANIMAL_HPP
#define ANIMAL_HPP
#include <string>

class Animal
{
private:
  int age; // UndCC_Violation
  std::string name; // name for the given animal
  std::string species; // UndCC_Violation
  
public:
  /* Default constructor */
  Animal();
  int get_age(); // UndCC_Violation
  void set_age(int);  // this function allows user to specify an age for the animal
  std::string get_name(); // UndCC_Violation
  void set_name(std::string); // UndCC_Violation
  // this comment describes the method get_species
  std::string get_species();
  void set_species(std::string); // UndCC_Violation
};

#endif
