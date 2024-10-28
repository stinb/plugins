
#ifndef ANIMAL_HPP
#define ANIMAL_HPP
#include <string>

class Animal
{
private:
  // declaring the data members ( see order )
  int age;
  std::string name;
  std::string species;
  
public:
  Animal();
  Animal(int, std::string, std::string);
  int get_age();
  void set_age(int);
  std::string get_name();
  void set_name(std::string);
  std::string get_species();
  void set_species(std::string);
};

#endif
