
#include <string>
#include "animal.hpp"

// Valid default constructor
c_Animal::c_Animal() : age(0), name("unnamed"), species("unknown")
{
  // code goes here
}

int c_Animal::get_age()
{
  return age;
}

void c_Animal::set_age(int age_input)
{
  age = age_input;
}

std::string c_Animal::get_name() 
{
  return name;
}

void c_Animal::set_name(std::string name_input)
{
  name = name_input;
}

std::string c_Animal::get_species()
{
  return species;
}

void c_Animal::set_species(std::string species_input)
{
  species = species_input;
}
