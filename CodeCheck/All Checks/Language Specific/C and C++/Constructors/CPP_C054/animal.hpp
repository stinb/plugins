#ifndef ANIMAL_HPP
#define ANIMAL_HPP
#include <string>
using namespace std;

/* This class will not be flagged as it provides a copy constructor */
class Animal
{
private:
  int m_age;
  string m_species;

public:
  Animal(int age, string species);

  // Copy constructor
  Animal(const Animal &a2) { m_age = a2.m_age; m_species = a2.m_species; }

  void SetAge(int age);

  int getAge() { return m_age; }
  string getSpecies() { return m_species; }
};

#endif
