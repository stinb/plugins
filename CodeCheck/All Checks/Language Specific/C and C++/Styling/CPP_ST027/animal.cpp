
#include <string>
#include "animal.hpp"

// default constructor
Animal::Animal() : d_age(0), name_d_("unnamed"), species("unknown")
{
  // code goes here
}

int Animal::m_get_age()
{
  return d_age;
}

void Animal::m_set_age(int age_input)
{
  d_age = age_input;
}

std::string Animal::get_name() 
{
  return name_d_;
}

void Animal::set_name(std::string name_input)
{
  name_d_ = name_input;
}

std::string Animal::get_species()
{
  return species;
}

void Animal::set_species(std::string species_input)
{
  species = species_input;
}
