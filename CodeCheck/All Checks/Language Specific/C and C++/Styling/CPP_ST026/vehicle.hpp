
#ifndef VEHICLE_HPP
#define VEHICLE_HPP
#include <string>

class Vehicle // UndCC_Violation
{
private:
  int year;
  std::string make;
  std::string model;
  
public:
  /* Default constructor */
  Vehicle();
  int get_year();
  void set_year(int);
};

#endif
