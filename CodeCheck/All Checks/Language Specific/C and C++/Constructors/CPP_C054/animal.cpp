#include "animal.hpp"

// Animal constructor
Animal::Animal(int age, string species)
{
  SetAge(age);
  m_species = species;
}

// Animal member function
void Animal::SetAge(int age)
{
  m_age = age;
}
