
#include <string>
#include "animal.hpp"

// Valid default constructor
Animal::Animal() : age(0), name("unnamed"), species("unknown")
{
  // code goes here
}

int Animal::get_age()
{
  return age;
}

void Animal::set_age(int age_input)
{
  age = age_input;
}

std::string Animal::get_name() 
{
  return name;
}

void Animal::set_name(std::string name_input)
{
  name = name_input;
}

std::string Animal::get_species()
{
  return species;
}

void Animal::set_species(std::string species_input)
{
  species = species_input;
}
