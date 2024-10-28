
#include <string>
#include "vehicle.hpp"

// Valid default constructor
Vehicle::Vehicle() : year(2000), make("Honda"), model("Civic")
{
  // code goes here
}

int Vehicle::get_year()
{
  return year;
}

void Vehicle::set_year(int year_input)
{
  year = year_input;
}
