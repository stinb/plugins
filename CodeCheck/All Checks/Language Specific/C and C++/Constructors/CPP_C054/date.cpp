#include "date.hpp"

// Date constructor
Date::Date(int year, int month, int day)
{
  SetDate(year, month, day);
}

// Date member function
void Date::SetDate(int year, int month, int day)
{
  m_month = month;
  m_day = day;
  m_year = year;
}
