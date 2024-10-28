
#ifndef ANIMAL_HPP
#define ANIMAL_HPP
#include <string>

class c_Animal
{
private:
  int age;
  std::string name;
  std::string species;
  
public:
  /* Default constructor */
  c_Animal();
  int get_age();
  void set_age(int);
  std::string get_name();
  void set_name(std::string);
  std::string get_species();
  void set_species(std::string);
};

#endif
